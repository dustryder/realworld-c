#include "unity.h"
#include "lib_tests.h"
#include "../src/lib/string_helpers.h"

void test_sluggify_should_turn_string_into_slug(void) {

    // Arrange
    char *title = "My New Title!";
    int title_count = 0;
    char *expected_result = "my-new-title-0";

    // Act
    char *result = sluggify(title, title_count);

    // Assert
    TEST_ASSERT_EQUAL_STRING(result, expected_result);
}

void run_lib_tests(void) {
    RUN_TEST(test_sluggify_should_turn_string_into_slug);
}