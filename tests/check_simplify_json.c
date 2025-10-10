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

START_TEST(test_one_line) {
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

START_TEST(test_multiple_line) {
    // Arrange
    char expected[] = "{\"user\":\"Alex\",\"age\":18}";
    char test_json[] =
        "{\n"
        "  \"user\": \"Alex\"\n,"
        "  \"age\": 18\n"
        "}";

    // Act
    char *simplified_json = simplify_json(test_json);

    // Assert
    ck_assert_str_eq(simplified_json, expected);
}
END_TEST

START_TEST(test_list) {
    // Arrange
    char expected[] = "{\"list\":[1,2,3]}";
    char test_json[] =
        "{\n"
        "  \"list\": [1, 2 ,3]\n"
        "}";

    // Act
    char *simplified_json = simplify_json(test_json);

    // Assert
    ck_assert_str_eq(simplified_json, expected);
}

START_TEST(test_empty_object) {
    // Arrange
    char expected[] = "{}";
    char test_json[] = "{   \n   \t  }";

    // Act
    char *simplified_json = simplify_json(test_json);

    // Assert
    ck_assert_str_eq(simplified_json, expected);
}
END_TEST

START_TEST(test_empty_array) {
    // Arrange
    char expected[] = "[]";
    char test_json[] = "[   \n   \t  ]";

    // Act
    char *simplified_json = simplify_json(test_json);

    // Assert
    ck_assert_str_eq(simplified_json, expected);
}
END_TEST

START_TEST(test_nested_objects_arrays) {
    // Arrange
    char expected[] = "{\"a\":{\"b\":[1,2,{\"c\":3}]}}";
    char test_json[] =
        "{ \n"
        "  \"a\"  :  { \n"
        "    \"b\" : [ 1 , 2 , { \"c\" : 3 } ]\n"
        "  }\n"
        "}";

    // Act
    char *simplified_json = simplify_json(test_json);

    // Assert
    ck_assert_str_eq(simplified_json, expected);
}
END_TEST

START_TEST(test_string_internal_spaces_preserved) {
    // Arrange
    char expected[] = "{\"s\":\"  a  b   c \\t d  \"}";
    char test_json[] =
        "{\n"
        "  \"s\" : \"  a  b   c \\t d  \"\n"
        "}";

    // Act
    char *simplified_json = simplify_json(test_json);

    // Assert
    ck_assert_str_eq(simplified_json, expected);
}
END_TEST

START_TEST(test_escaped_quotes_and_backslash) {
    // Arrange
    char expected[] = "{\"q\":\"He said \\\"hi\\\\bye\\\"\"}";
    char test_json[] =
        "{\n"
        "  \"q\": \"He said \\\"hi\\\\bye\\\"\"\n"
        "}";

    // Act
    char *simplified_json = simplify_json(test_json);

    // Assert
    ck_assert_str_eq(simplified_json, expected);
}
END_TEST

START_TEST(test_unicode_escape_sequences) {
    // Arrange
    char expected[] =
        "{\"u\":\"\\u20AC\\u041F\\u0440\\u0438\\u0432\\u0435\\u0442\"}";
    char test_json[] =
        "{\n"
        "  \"u\" : \"\\u20AC\\u041F\\u0440\\u0438\\u0432\\u0435\\u0442\"\n"
        "}";

    // Act
    char *simplified_json = simplify_json(test_json);

    // Assert
    ck_assert_str_eq(simplified_json, expected);
}
END_TEST

START_TEST(test_numbers_booleans_null) {
    // Arrange
    char expected[] = "{\"n\":-12.34e+10,\"b\":true,\"z\":null}";
    char test_json[] =
        "{  \"n\"  :  -12.34e+10 ,  \"b\" :  true , \"z\" :  null }";

    // Act
    char *simplified_json = simplify_json(test_json);

    // Assert
    ck_assert_str_eq(simplified_json, expected);
}
END_TEST

START_TEST(test_whitespace_variants) {
    // Arrange
    char expected[] = "{\"a\":1,\"b\":2}";
    char test_json[] =
        "{\r\n"
        "\t\"a\"  :  1 ,\r"
        " \t \"b\" :\n"
        " 2 \n"
        "}\n";

    // Act
    char *simplified_json = simplify_json(test_json);

    // Assert
    ck_assert_str_eq(simplified_json, expected);
}
END_TEST

START_TEST(test_already_minified) {
    // Arrange
    char expected[] = "{\"k\":[1,2,3]}";
    char test_json[] = "{\"k\":[1,2,3]}";

    // Act
    char *simplified_json = simplify_json(test_json);

    // Assert
    ck_assert_str_eq(simplified_json, expected);
}
END_TEST

START_TEST(test_large_array_spaces) {
    // Arrange
    char expected[] = "{\"arr\":[1,2,3,4,5,6,7,8,9,10]}";
    char test_json[] =
        "{ \"arr\" : [ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 ] }";

    // Act
    char *simplified_json = simplify_json(test_json);

    // Assert
    ck_assert_str_eq(simplified_json, expected);
}
END_TEST

START_TEST(test_colon_comma_spacing) {
    // Arrange
    char expected[] = "{\"a\":1,\"b\":{\"c\":2},\"d\":[3,4]}";
    char test_json[] =
        "{ \"a\"  :  1  ,  \"b\" : { \"c\"  : 2 } ,  \"d\" : [ 3 , 4 ] }";

    // Act
    char *simplified_json = simplify_json(test_json);

    // Assert
    ck_assert_str_eq(simplified_json, expected);
}
END_TEST

START_TEST(test_braces_in_string_unchanged) {
    // Arrange
    char expected[] = "{\"s\":\"{ not a brace } [ nor array ] : ,\"}";
    char test_json[] =
        "{\n"
        "  \"s\" : \"{ not a brace } [ nor array ] : ,\"\n"
        "}";

    // Act
    char *simplified_json = simplify_json(test_json);

    // Assert
    ck_assert_str_eq(simplified_json, expected);
}
END_TEST

START_TEST(test_deep_mixed_structure) {
    // Arrange
    char expected[] =
        "{\"m\":{\"x\":[{\"y\":[1,2,{\"z\":\"ok\"}]}],\"k\":false}}";
    char test_json[] =
        "{\n"
        "  \"m\" : {\n"
        "    \"x\" : [ { \"y\" : [ 1 , 2 , { \"z\" : \"ok\" } ] } ],\n"
        "    \"k\" : false\n"
        "  }\n"
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
    tcase_add_test(tc_core, test_one_line);
    tcase_add_test(tc_core, test_multiple_line);
    tcase_add_test(tc_core, test_list);
    tcase_add_test(tc_core, test_empty_object);
    tcase_add_test(tc_core, test_empty_array);
    tcase_add_test(tc_core, test_nested_objects_arrays);
    tcase_add_test(tc_core, test_string_internal_spaces_preserved);
    tcase_add_test(tc_core, test_escaped_quotes_and_backslash);
    tcase_add_test(tc_core, test_unicode_escape_sequences);
    tcase_add_test(tc_core, test_numbers_booleans_null);
    tcase_add_test(tc_core, test_whitespace_variants);
    tcase_add_test(tc_core, test_already_minified);
    tcase_add_test(tc_core, test_large_array_spaces);
    tcase_add_test(tc_core, test_colon_comma_spacing);
    tcase_add_test(tc_core, test_braces_in_string_unchanged);
    tcase_add_test(tc_core, test_deep_mixed_structure);
    suite_add_tcase(s, tc_core);

    return s;
}