#include "global_incorrect_line_endings_count_logger.h"
#include <stdio.h>
#include "global_incorrect_line_endings_count/global_incorrect_line_endings_count.h"

void log_global_incorrect_line_endings_count(void)
{	printf("processed %i incorrect line endings\n", global_incorrect_line_endings_count);
}
