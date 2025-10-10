#include <check.h>

#include "format.h"

START_TEST(test_format_empty_object_no_change) {
    // Arrange
    char expected[] = "{}";
    char test_json[] = "{   \n   \t  }";

    // Act
    char *formatted = format_json(test_json);

    // Assert
    ck_assert_str_eq(formatted, expected);
}
END_TEST

START_TEST(test_format_simple_object) {
    // Arrange
    char expected[] =
        "{\n"
        "  \"a\": 1\n"
        "}";
    char test_json[] = "{  \"a\"  :  1 }";

    // Act
    char *formatted = format_json(test_json);

    // Assert
    ck_assert_str_eq(formatted, expected);
}
END_TEST

START_TEST(test_format_top_array_numbers) {
    // Arrange
    char expected[] =
        "[\n"
        "  1,\n"
        "  2,\n"
        "  3\n"
        "]";
    char test_json[] = "[1,2,3]";

    // Act
    char *formatted = format_json(test_json);

    // Assert
    ck_assert_str_eq(formatted, expected);
}
END_TEST

START_TEST(test_format_object_with_array) {
    // Arrange
    char expected[] =
        "{\n"
        "  \"a\": [\n"
        "    1,\n"
        "    2\n"
        "  ]\n"
        "}";
    char test_json[] = "{\n  \"a\"  :  [ 1 , 2 ]\n}";

    // Act
    char *formatted = format_json(test_json);

    // Assert
    ck_assert_str_eq(formatted, expected);
}
END_TEST

START_TEST(test_format_empty_array_current_behavior) {
    // Arrange
    char expected[] =
        "[\n"
        "  \n"
        "]";
    char test_json[] = "[]";

    // Act
    char *formatted = format_json(test_json);

    // Assert
    ck_assert_str_eq(formatted, expected);
}
END_TEST

START_TEST(test_format_nested_with_empty_object) {
    // Arrange
    char expected[] =
        "{\n"
        "  \"obj\": {}\n"
        "}";
    char test_json[] = "{ \"obj\" : { } }";

    // Act
    char *formatted = format_json(test_json);

    // Assert
    ck_assert_str_eq(formatted, expected);
}
END_TEST

START_TEST(test_format_commas_and_colons) {
    // Arrange
    char expected[] =
        "{\n"
        "  \"a\": 1,\n"
        "  \"b\": 2,\n"
        "  \"c\": 3\n"
        "}";
    char test_json[] = "{\n \"a\"  :1 , \"b\":2,  \"c\"  :3 }";

    // Act
    char *formatted = format_json(test_json);

    // Assert
    ck_assert_str_eq(formatted, expected);
}
END_TEST

START_TEST(test_format_deep_mixed) {
    // Arrange
    char expected[] =
        "{\n"
        "  \"m\": {\n"
        "    \"x\": [\n"
        "      {\n"
        "        \"y\": [\n"
        "          1,\n"
        "          2,\n"
        "          {\n"
        "            \"z\": \"ok\"\n"
        "          }\n"
        "        ]\n"
        "      }\n"
        "    ],\n"
        "    \"k\": false\n"
        "  }\n"
        "}";
    char test_json[] =
        "{ \"m\" : { \"x\" : [ { \"y\" : [ 1 , 2 , { \"z\" : \"ok\" } ] } ] , "
        "\"k\": false } }";

    // Act
    char *formatted = format_json(test_json);

    // Assert
    ck_assert_str_eq(formatted, expected);
}
END_TEST

START_TEST(test_format_final_closing_brace_indent) {
    // Arrange
    char expected[] =
        "{\n"
        "  \"a\": {\n"
        "    \"b\": 1\n"
        "  }\n"
        "}";
    char test_json[] = "{ \"a\" : { \"b\" : 1 } }";

    // Act
    char *formatted = format_json(test_json);

    // Assert
    ck_assert_str_eq(formatted, expected);
}
END_TEST

START_TEST(test_format_array_of_objects_with_empty) {
    // Arrange
    char expected[] =
        "[\n"
        "  {\"a\": 1},\n"
        "  {},\n"
        "  {\"b\": 2}\n"
        "]";
    char test_json[] = "[ {\"a\":1} , { } , { \"b\" : 2 } ]";

    // Act
    char *formatted = format_json(test_json);

    // Assert
    ck_assert_str_eq(formatted, expected);
}
END_TEST

Suite *json_format_suite(void) {
    Suite *s = suite_create("FORMAT");

    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_format_empty_object_no_change);
    tcase_add_test(tc_core, test_format_simple_object);
    tcase_add_test(tc_core, test_format_top_array_numbers);
    tcase_add_test(tc_core, test_format_object_with_array);
    tcase_add_test(tc_core, test_format_empty_array_current_behavior);
    tcase_add_test(tc_core, test_format_nested_with_empty_object);
    tcase_add_test(tc_core, test_format_commas_and_colons);
    tcase_add_test(tc_core, test_format_deep_mixed);
    tcase_add_test(tc_core, test_format_final_closing_brace_indent);
    tcase_add_test(tc_core, test_format_array_of_objects_with_empty);
    suite_add_tcase(s, tc_core);

    return s;
}
