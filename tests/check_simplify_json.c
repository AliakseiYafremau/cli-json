#include <check.h>

#include "json.h"

START_TEST(test_not_json) {
    // Arrange
    char expected[] = "1234";
    char test_json[] = "1234";

    // Act
    char *simplified_json = simplify_json(test_json);

    // Assert
    ck_assert_str_eq(simplified_json, expected);
}
END_TEST

START_TEST(test_one_line_json) {
    // Arrange
    char expected[] = "{\"user\":\"Alex\"}";
    char test_json[] =
        "{\n"
        "  \"user\": \"Alex\"\n"
        "}";

    // Act
    char *simplified_json = simplify_json(test_json);

    // Assert
    ck_assert_str_eq(simplified_json, expected);
}
END_TEST

Suite *json_suite(void) {
    Suite *s = suite_create("JSON");

    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_not_json);
    tcase_add_test(tc_core, test_one_line_json);
    suite_add_tcase(s, tc_core);

    return s;
}