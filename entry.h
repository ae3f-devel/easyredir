#include <ae2f/Keys.h>

/**
 * @fn		easypipe_entry
 * @brief	entry for easy pipe
 * @param	rd_path_istream	path to stdin redirection
 * 		> when blank ("") redirection should not be happening
 *
 * @param	rd_path_ostream	path to stdout redirection
 * 		> when blank ("") redirection should not be happening
 *
 * @param	rd_path_estream	path to stderr redirection
 * 		> when blank ("") redirection should not be happening
 *
 * @param	rd_process	path to process entry (programme)
 *
 * @param	c_is_append	whether to append the output on file?
 * 				when 0, file will be completely overwritten.
 *
 * @param	c_argc		argument count **excluding** programme name
 * @param	rd_argv		argumetns
 * */
int easyredir_entry(
		const char* ae2f_restrict const rd_path_istream,
		const char* ae2f_restrict const rd_path_ostream,
		const char* ae2f_restrict const rd_path_estream,
		const char* ae2f_restrict const rd_process,
		const int c_is_append,
		const int c_argc, const char** ae2f_restrict rd_argv
		);
