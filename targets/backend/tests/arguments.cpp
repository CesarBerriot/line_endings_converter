#include <gtest/gtest.h>
#include <backend.hpp>
#include <hard_assert.hpp>
#include <pre_main_runner.hpp>
#include <make_string.hpp>
#include <cstdlib>
#include <cstdarg>

RUN_PRE_MAIN
(	ha_require_user_interaction = false;
)

TEST(arguments, correct)
{	auto expect_results = [&](char line_ending[], ...)
	{	va_list variadic_arguments;
		va_start(variadic_arguments, line_ending);

		testing::internal::CaptureStdout();

		char * program_arguments[] = { "", line_ending, "test_directory" };
		ASSERT_EQ(run(program_arguments, 3), EXIT_SUCCESS);

		ASSERT_STREQ
		(	testing::internal::GetCapturedStdout().data(),
			make_string_variadic
			(	"processing file : 'test_directory\\file1'\n"
				"found %i incorrect line endings\n"
				"\n"
				"processing file : 'test_directory\\file2'\n"
				"found %i incorrect line endings\n"
				"\n"
				"processing file : 'test_directory\\sub_directory\\file3'\n"
				"found %i incorrect line endings\n"
				"\n"
				"processed %i incorrect line endings\n",
				variadic_arguments
			)
		);

		va_end(variadic_arguments);
	};

	char * line_endings[] = { "CRLF", "CR", "LF" };
	enum { line_endings_length = sizeof(line_endings) / sizeof(line_endings[0]) };
	for(int i = 0; i < line_endings_length; ++i)
	{	char * line_ending = line_endings[i];
		expect_results(line_ending, 2, 1, 3, 6);
		expect_results(line_ending, 0, 0, 0, 0);
	}
}

TEST(arguments, invalid_line_ending)
{	char * arguments[] = { "", "ABCD", "test_directory" };
	ASSERT_EQ(run(arguments, 3), EXIT_FAILURE);
}

TEST(arguments, invalid_path)
{	char * arguments[] = { "", "CRLF", "!?*+\\/:';" };
	ASSERT_EQ(run(arguments, 3), EXIT_FAILURE);
}

TEST(arguments, not_enough)
{	for(int i = -1; i < 3; ++i)
		EXPECT_EQ(run(NULL, i), EXIT_FAILURE);
}

TEST(arguments, too_many)
{	for(int i = 4; i < 10; ++i)
		EXPECT_EQ(run(NULL, i), EXIT_FAILURE);
}
