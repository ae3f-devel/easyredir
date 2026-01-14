#include "./entry.h"

#if !(defined(_WIN32) && _WIN32)

int easypipe_entry(
		const char* ae2f_restrict const rd_path_istream,
		const char* ae2f_restrict const rd_path_ostream,
		const char* ae2f_restrict const rd_path_estream,
		const char* ae2f_restrict const rd_process,
		const int c_is_append,
		const int c_argc, const char** ae2f_restrict rd_argv
		)
{
	(void)rd_path_estream;
	(void)rd_path_istream;
	(void)rd_path_ostream;
	(void)rd_process;
	(void)c_is_append;
	(void)c_argc;
	(void)rd_argv;

	return 0;
}

#endif
