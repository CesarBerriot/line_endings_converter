#include "usage_logger.h"
#include <stdio.h>
#include "line_endings/line_endings.h"

void log_usage(void)
{	puts
	(	"USAGE\n"
		"line_endings_converter DESIRED_LINE_ENDING PATH\n"
		"e.g. line_endings_converter CRLF path/to/my/directory\n"
		"\n"
		"AVAILABLE LINE ENDINGS"
	);
	for(int i = 0; i < line_endings_length; ++i)
		printf("%s (%s)\n", line_endings[i].name, line_endings[i].description);
}
