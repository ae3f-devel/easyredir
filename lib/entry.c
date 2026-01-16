#include "./easyredir.h"


EASYREDIR_IMPL int easyredir_entry(
		const char* ae2f_restrict const rd_path_istream,
		const char* ae2f_restrict const rd_path_ostream,
		const char* ae2f_restrict const rd_path_estream,
		const char* ae2f_restrict const rd_process,
		const int c_is_append,
		const int c_argc, const char** ae2f_restrict rd_argv
		)
{
	return easyredir_entry2(
			rd_path_istream
			, rd_path_ostream
			, rd_path_estream
			, ""
			, rd_process
			, c_is_append
			, c_argc
			, rd_argv
			);
}
