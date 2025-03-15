#include "file_processor.h"
#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <string.h>
#include <hard_assert.h>
#include <make_string.h>
#include "settings/settings.h"
#include "line_endings/line_endings.h"
#include "global_incorrect_line_endings_count/global_incorrect_line_endings_count.h"

struct line_ending_research_results
{	bool succeeded;
	char * found_line_ending;
	bool correctness;
};

static char * path;
static FILE * file;
static int file_size;
static char * input_buffer;
static int incorrect_line_endings_count;

static void log_file(void);
static void open_file_for_reading(void);
static void compute_file_size(void);
static void read_file(void);
static void count_incorrect_line_endings(void);
static void log_incorrect_line_endings(void);
static void update_global_incorrect_line_endings_count(void);
static void reopen_file_for_writing(void);
static void write_file_with_corrected_line_endings(void);
static void close_file(void);
static void log_return(void);

static void assert(bool condition, char cause[]);
struct line_ending_research_results has_line_ending(int index);
static void iterate_input_buffer_elements(void(*character_callback)(char), void(*line_ending_callback)(bool correctness));
static void update_incorrect_line_endings_count(bool correctness);
static void write_character(char);
static void write_selected_line_ending(bool);

void process_file(char path_argument[])
{	path = path_argument;

	log_file();
	open_file_for_reading();
	compute_file_size();
	read_file();
	count_incorrect_line_endings();
	log_incorrect_line_endings();
	update_global_incorrect_line_endings_count();
	reopen_file_for_writing();
	write_file_with_corrected_line_endings();
	close_file();
	log_return();
}

static void log_file(void)
{	printf("processing file : '%s'\n", path);
}

static void open_file_for_reading(void)
{	file = fopen(path, "rb");
	assert(file, "fopen");
}

static void compute_file_size(void)
{	assert(!fseek(file, 0, SEEK_END), "fseek SEEK_END");
	file_size = ftell(file);
	assert(file_size != -1, "ftell");
	assert(!fseek(file, 0, SEEK_SET), "fseek SEEK_SET");
}

static void read_file(void)
{	input_buffer = malloc(file_size);
	assert(input_buffer, "Memory allocation");
	int read_bytes_count = fread(input_buffer, 1, file_size, file);
	assert(read_bytes_count == file_size, "fread");
}

static void count_incorrect_line_endings(void)
{	incorrect_line_endings_count = 0;
	iterate_input_buffer_elements(NULL, update_incorrect_line_endings_count);
}

static void log_incorrect_line_endings(void)
{	printf("found %i incorrect line endings\n", incorrect_line_endings_count);
}

static void update_global_incorrect_line_endings_count(void)
{	global_incorrect_line_endings_count += incorrect_line_endings_count;
}

static void reopen_file_for_writing(void)
{	assert(freopen(path, "wb", file) == file, "freopen");
}

static void write_file_with_corrected_line_endings(void)
{	iterate_input_buffer_elements(write_character, write_selected_line_ending);
}

static void close_file(void)
{	assert(!fclose(file), "fclose");
}

static void log_return(void)
{	putchar('\n');
}

static void assert(bool condition, char cause[])
{	ha_assert(condition, MODULE_NAME, make_string("%s failure for file '%s'.", cause, path));
}

struct line_ending_research_results has_line_ending(int index)
{	char * input = &input_buffer[index];

	for(int i = 0; i < line_endings_length; ++i)
	{	char * line_ending = line_endings[i].content;
		if(!strncmp(input, line_ending, strlen(line_ending)))
			return (struct line_ending_research_results)
			{	.succeeded = true,
				.found_line_ending = line_ending,
				.correctness = !strcmp(line_ending, settings.selected_line_ending)
			};
	}

	return (struct line_ending_research_results){ .succeeded = false };
}

static void iterate_input_buffer_elements(void(*character_callback)(char), void(*line_ending_callback)(bool correctness))
{	for(int i = 0; i < file_size;)
	{	struct line_ending_research_results line_ending_research_results = has_line_ending(i);
		if(line_ending_research_results.succeeded)
		{	if(line_ending_callback)
				line_ending_callback(line_ending_research_results.correctness);
			i += strlen(line_ending_research_results.found_line_ending);
		}
		else
		{	if(character_callback)
				character_callback(input_buffer[i]);
			++i;
		}
	}
}

static void update_incorrect_line_endings_count(bool correctness)
{	incorrect_line_endings_count += !correctness;
}

static void write_character(char character)
{	assert(fputc(character, file) == character, "fputc");
}

static void write_selected_line_ending(bool line_ending_correctness)
{	int length = strlen(settings.selected_line_ending);
	assert(fwrite(settings.selected_line_ending, 1, length, file) == length, "fwrite");
}
