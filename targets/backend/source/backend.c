#include "backend.h"
#include <stdlib.h>
#include "argument_parser/argument_parser.h"
#include "directory_processor/directory_processor.h"
#include "usage_logger/usage_logger.h"
#include "global_incorrect_line_endings_count/global_incorrect_line_endings_count.h"
#include "global_incorrect_line_endings_count_logger/global_incorrect_line_endings_count_logger.h"

static void reset_global_incorrect_line_endings_count(void);

int run(char ** arguments, int arguments_length)
{	if(parse_arguments(arguments, arguments_length))
	{	reset_global_incorrect_line_endings_count();
		process_selected_directory();
		log_global_incorrect_line_endings_count();
		return EXIT_SUCCESS;
	}
	else
	{	log_usage();
		return EXIT_FAILURE;
	}
}

static void reset_global_incorrect_line_endings_count(void)
{	global_incorrect_line_endings_count = 0;
}
