#include "argument_parser.h"
#include <errno.h>
#include <sys/stat.h>
#include <hard_assert.h>
#include <string.h>
#include "settings/settings.h"
#include "line_endings/line_endings.h"

static char ** arguments;
static int arguments_length;
static char * selected_line_ending;

bool check_arguments_length(void);
void compute_selected_line_ending(void);
bool check_selected_line_ending(void);
bool check_path(void);

bool parse_arguments(char ** arguments_argument, int arguments_length_argument)
{	arguments = arguments_argument;
	arguments_length = arguments_length_argument;

	if(!check_arguments_length())
		return false;

	compute_selected_line_ending();

	if(!check_selected_line_ending() || !check_path())
		return false;

	settings = (struct settings)
	{	.selected_line_ending = selected_line_ending,
		.path = arguments[2]
	};

	return check_selected_line_ending() && check_path();
}

bool check_arguments_length(void)
{	return arguments_length == 3;
}

void compute_selected_line_ending(void)
{	for(int i = 0; i < line_endings_length; ++i)
		if(!strcmp(arguments[1], line_endings[i].name))
		{	selected_line_ending = line_endings[i].content;
			return;
		}
	selected_line_ending = NULL;
}

bool check_selected_line_ending(void)
{	return selected_line_ending;
}

bool check_path(void)
{	struct stat report;
	if(stat(arguments[2], &report))
	{	ha_assert(errno == ENOENT, MODULE_NAME, "stat failure.");
		return false;
	}
	else
		return true;
}
