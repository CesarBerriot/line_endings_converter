#include "line_endings.h"

struct line_ending line_endings[] =
{	{	.name = "CRLF",
		.description = "Windows",
		.content = "\r\n"
	},
	{	.name = "LF",
		.description = "Unix",
		.content = "\n"
	},
	{	.name = "CR",
		.description = "Mac",
		.content = "\r"
	}
};

int line_endings_length = sizeof(line_endings) / sizeof(line_endings[0]);