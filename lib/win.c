#include "./easyredir.h"

#if (defined(_WIN32) && _WIN32)

/** TODO: complete implementation */
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
	(void)rd_path_estream;
	(void)rd_path_istream;
	(void)rd_path_ostream;
	(void)rd_process;
	(void)c_is_append;
	(void)c_argc;
	(void)rd_argv;
	(void)rd_workdir;

	return 0;
}

#endif
