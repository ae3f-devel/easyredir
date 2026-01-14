#include "./entry.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char** argv);
int main(int argc, const char** argv) {
	if(argc < 6) {
		puts("easyredir stdin_file stdout_file stderr_file is_append programme-to-execute __VA_ARGS__|null");
		return 0;
	}

	return easyredir_entry(
			argv[1]
			, argv[2]
			, argv[3]
			, argv[5]
			, atoi(argv[4])
			, argc - 6
			, argv + 6
			);
}
