#include "./entry.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char** argv);
int main(int argc, const char** argv) {
	if(argc < 6) {
		puts("easypipe stdin_file stdout_file stderr_file programme-to-execute __VA_ARGS__|null");
		return 0;
	}

	return easypipe_entry(
			argv[1]
			, argv[2]
			, argv[3]
			, argv[4]
			, atoi(argv[5])
			, argc - 6
			, argv + 6
			);
}
