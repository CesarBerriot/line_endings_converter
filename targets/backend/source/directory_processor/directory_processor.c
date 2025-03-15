#include "directory_processor.h"
#include <sys/stat.h>
#include <stdbool.h>
#include <malloc.h>
#include <dirent.h>
#include <hard_assert.h>
#include <make_string.h>
#include "file_processor/file_processor.h"
#include "settings/settings.h"

static void process_directory(char[]);

static DIR * open_directory(char[]);
static void skip_navigation_entries(DIR*);
static void process_entries(char[], DIR*);
static void close_directory(DIR*);

static void process_entry(char[]);
static bool is_directory(char[]);

void process_selected_directory(void)
{	process_directory(settings.path);
}

void process_directory(char path[])
{	DIR * directory = open_directory(path);
	skip_navigation_entries(directory);
	process_entries(path, directory);
	close_directory(directory);
}

static DIR * open_directory(char path[])
{	DIR * directory = opendir(path);
	ha_assert(directory, MODULE_NAME, make_string("opendir failed on path '%s'", path));
	return directory;
}

static void skip_navigation_entries(DIR * directory)
{	ha_assert(readdir(directory), MODULE_NAME, "failed to skip directory entry '.'");
	ha_assert(readdir(directory), MODULE_NAME, "failed to skip directory entry '.'");
}

static void process_entries(char path[], DIR * directory)
{	for(struct dirent * entry = readdir(directory); entry; entry = readdir(directory))
	{	char * entry_path = strdup(make_string("%s\\%s", path, entry->d_name));
		process_entry(entry_path);
		free(entry_path);
	}
}

static void close_directory(DIR * directory)
{	ha_assert(!closedir(directory), MODULE_NAME, "closedir failure in function close_directory");
}

static void process_entry(char path[])
{	if(is_directory(path))
		process_directory(path);
	else
		process_file(path);
}

static bool is_directory(char path[])
{	struct stat report;
	ha_assert(!stat(path, &report), MODULE_NAME, make_string("stat failed on path '%s'", path));
	return S_ISDIR(report.st_mode);
}
