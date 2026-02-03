#include "./easyredir.h"

#if !(defined(_WIN32) && _WIN32)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/mman.h>
#include <sys/wait.h>

#include <unistd.h>
#include <assert.h>
#include <fcntl.h>

EASYREDIR_IMPL int easyredir_entry2(
		const char* ae2f_restrict const rd_path_istream,
		const char* ae2f_restrict const rd_path_ostream,
		const char* ae2f_restrict const rd_path_estream,
		const char* ae2f_restrict const rd_workdir,
		const char* ae2f_restrict const rd_process,
		const int c_is_append,
		const int c_argc, const char** ae2f_restrict rd_argv
		)
{
	pid_t	PID;
	int	RET = 0, PROC_RET;
	int	ISTREAM = -1;
	int	OSTREAM = -1;
	int	ESTREAM = -1;

	char** ae2f_restrict const RD_ARGV = 
		malloc((size_t)((size_t)(c_argc + 2) * (size_t)sizeof(void*)));
#define jmpreturn(a)	{ RET = (a); goto LBL_RET; }

	assert(rd_path_istream);
	assert(rd_path_ostream);
	assert(rd_path_estream);
	assert(rd_process);
	assert(*rd_process);
	assert(rd_workdir);

	ae2f_unexpected_but_if(RD_ARGV == 0) {
		assert(0 && "mmap");
		jmpreturn(-1);
	}

	if(rd_path_istream[0]) {
		ISTREAM = open(rd_path_istream, O_RDONLY);

		ae2f_unexpected_but_if(ISTREAM == -1) {
			assert(0 && "ISTREAM open has failed.");
			jmpreturn(-1);
		}
	}

	if(rd_path_ostream[0]) {
		OSTREAM = open(rd_path_ostream, O_CREAT | O_WRONLY 
				| (c_is_append ? O_APPEND : 0)
				, 0666);

		ae2f_unexpected_but_if(OSTREAM == -1) {
			assert(0 && "OSTREAM open has failed.");
			jmpreturn(-1);
		}
	}

	if(rd_path_estream[0]) {
		ESTREAM = open(rd_path_estream, O_CREAT | O_WRONLY 
				| (c_is_append ? O_APPEND : 0)
				, 0666);

		ae2f_unexpected_but_if(ESTREAM == -1) {
			assert(0 && "ESTREAM open has failed.");
			jmpreturn(-1);
		}
	}

	PID = fork();

	if (PID < 0) {
		assert(0 && "fork");
		jmpreturn(-1);
	}


	unless(PID) {
		/** this is child */
		if(ISTREAM != -1) {
			dup2(ISTREAM, STDIN_FILENO);
		} 

		if(OSTREAM != -1) {
			dup2(OSTREAM, STDOUT_FILENO);
		} 

		if(ESTREAM != -1) {
			dup2(ESTREAM, STDERR_FILENO);
		}

		memcpy(RD_ARGV + 1
				, rd_argv
				, (size_t)((size_t)(c_argc) * (size_t)sizeof(void*))
				);
		RD_ARGV[0] = (char*)rd_process;
		RD_ARGV[c_argc + 1] = 0;

		if(*rd_workdir) {
			chdir(rd_workdir);
		}

		execvp(rd_process, RD_ARGV);
		exit(127);
	}

	ae2f_unexpected_but_if(waitpid(PID, &PROC_RET, 0) == -1) {
		assert(0 && "waitpid has failed");
		jmpreturn(-1);
	}

	if (WIFEXITED(PROC_RET)) {
		jmpreturn(WEXITSTATUS(PROC_RET));
	}
	else if (WIFSIGNALED(PROC_RET)) {
#ifdef WCOREDUMP
		if (WCOREDUMP(PROC_RET))
			jmpreturn(-1);
#endif
		jmpreturn(WTERMSIG(PROC_RET));
	}
	else {
		jmpreturn(-1);
	}



LBL_RET:
	if(RD_ARGV)
		free(RD_ARGV);

	unless(ISTREAM == -1)
		close(ISTREAM);

	unless(OSTREAM == -1)
		close(OSTREAM);

	unless(ESTREAM == -1)
		close(ESTREAM);


	return RET;
}

#undef	jmpreturn

#endif
