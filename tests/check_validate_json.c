#include <check.h>

#include "json.h"

START_TEST(test_validate_simple_object) {
    char json[] = "{\"a\":1,\"b\":true}";
    ck_assert_int_eq(validate_json(json), 1);
}
END_TEST

START_TEST(test_validate_simple_array) {
    char json[] = "[1,2,3]";
    ck_assert_int_eq(validate_json(json), 1);
}
END_TEST

START_TEST(test_validate_primitives_top_level) {
    char json_true[] = "true";
    char json_false[] = "false";
    char json_null[] = "null";
    char json_num[] = "-12.5e+2";
    ck_assert_int_eq(validate_json(json_true), 1);
    ck_assert_int_eq(validate_json(json_false), 1);
    ck_assert_int_eq(validate_json(json_null), 1);
    ck_assert_int_eq(validate_json(json_num), 1);
}
END_TEST

START_TEST(test_validate_numbers_variants) {
    char json1[] = "0";
    char json2[] = "-0";
    char json3[] = "10";
    char json4[] = "10.01";
    char json5[] = "1e10";
    char json6[] = "1E-10";
    ck_assert_int_eq(validate_json(json1), 1);
    ck_assert_int_eq(validate_json(json2), 1);
    ck_assert_int_eq(validate_json(json3), 1);
    ck_assert_int_eq(validate_json(json4), 1);
    ck_assert_int_eq(validate_json(json5), 1);
    ck_assert_int_eq(validate_json(json6), 1);
}
END_TEST

START_TEST(test_validate_nested) {
    char json[] = "{\"a\":[{\"b\":false},null]}";
    ck_assert_int_eq(validate_json(json), 1);
}
END_TEST

START_TEST(test_validate_string_escapes) {
    char json[] = "{\"s\":\"quote:\\\" backslash:\\\\\"}";
    ck_assert_int_eq(validate_json(json), 1);
}
END_TEST

START_TEST(test_validate_unicode_escape) {
    char json[] = "{\"u\":\"\\u0041\"}";
    ck_assert_int_eq(validate_json(json), 1);
}
END_TEST

START_TEST(test_validate_empty_object_array) {
    char json1[] = "{}";
    char json2[] = "[]";
    ck_assert_int_eq(validate_json(json1), 1);
    ck_assert_int_eq(validate_json(json2), 1);
}
END_TEST

START_TEST(test_validate_string_with_brackets) {
    char json[] = "{\"s\":\"{[ ]}\"}";
    ck_assert_int_eq(validate_json(json), 1);
}
END_TEST

START_TEST(test_validate_invalid_trailing_comma_object) {
    char json[] = "{\"a\":1,}";
    ck_assert_int_eq(validate_json(json), 0);
}
END_TEST

START_TEST(test_validate_invalid_trailing_comma_array) {
    char json[] = "[1,2,]";
    ck_assert_int_eq(validate_json(json), 0);
}
END_TEST

START_TEST(test_validate_invalid_unclosed_string) {
    char json[] = "{\"a\":\"test}";
    ck_assert_int_eq(validate_json(json), 0);
}
END_TEST

START_TEST(test_validate_invalid_escape) {
    char json[] = "{\"a\":\"\\q\"}";
    ck_assert_int_eq(validate_json(json), 0);
}
END_TEST

START_TEST(test_validate_invalid_number) {
    char json[] = "{\"n\":01}";
    ck_assert_int_eq(validate_json(json), 0);
}
END_TEST

START_TEST(test_validate_invalid_number_variants) {
    char json1[] = "-";
    char json2[] = "1.";
    char json3[] = ".1";
    char json4[] = "1e";
    char json5[] = "1e+";
    char json6[] = "+1";
    ck_assert_int_eq(validate_json(json1), 0);
    ck_assert_int_eq(validate_json(json2), 0);
    ck_assert_int_eq(validate_json(json3), 0);
    ck_assert_int_eq(validate_json(json4), 0);
    ck_assert_int_eq(validate_json(json5), 0);
    ck_assert_int_eq(validate_json(json6), 0);
}
END_TEST

START_TEST(test_validate_invalid_literals) {
    char json1[] = "tru";
    char json2[] = "fals";
    char json3[] = "nul";
    ck_assert_int_eq(validate_json(json1), 0);
    ck_assert_int_eq(validate_json(json2), 0);
    ck_assert_int_eq(validate_json(json3), 0);
}
END_TEST

START_TEST(test_validate_invalid_structure_missing_comma) {
    char json[] = "{\"a\":1 \"b\":2}";
    ck_assert_int_eq(validate_json(json), 0);
}
END_TEST

START_TEST(test_validate_invalid_structure_missing_colon) {
    char json[] = "{\"a\" 1}";
    ck_assert_int_eq(validate_json(json), 0);
}
END_TEST

START_TEST(test_validate_invalid_unclosed_brackets) {
    char json1[] = "[1,2,3";
    char json2[] = "{\"a\":1";
    ck_assert_int_eq(validate_json(json1), 0);
    ck_assert_int_eq(validate_json(json2), 0);
}
END_TEST

START_TEST(test_validate_invalid_extra_tokens_after_value) {
    char json[] = "true false";
    ck_assert_int_eq(validate_json(json), 0);
}
END_TEST

START_TEST(test_validate_extra_content) {
    char json[] = "{}{}";
    ck_assert_int_eq(validate_json(json), 0);
}
END_TEST

Suite* json_validate_suite(void) {
    Suite* s = suite_create("VALIDATE");

    TCase* tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_validate_simple_object);
    tcase_add_test(tc_core, test_validate_simple_array);
    tcase_add_test(tc_core, test_validate_primitives_top_level);
    tcase_add_test(tc_core, test_validate_numbers_variants);
    tcase_add_test(tc_core, test_validate_nested);
    tcase_add_test(tc_core, test_validate_string_escapes);
    tcase_add_test(tc_core, test_validate_unicode_escape);
    tcase_add_test(tc_core, test_validate_empty_object_array);
    tcase_add_test(tc_core, test_validate_string_with_brackets);
    tcase_add_test(tc_core, test_validate_invalid_trailing_comma_object);
    tcase_add_test(tc_core, test_validate_invalid_trailing_comma_array);
    tcase_add_test(tc_core, test_validate_invalid_unclosed_string);
    tcase_add_test(tc_core, test_validate_invalid_escape);
    tcase_add_test(tc_core, test_validate_invalid_number);
    tcase_add_test(tc_core, test_validate_invalid_number_variants);
    tcase_add_test(tc_core, test_validate_invalid_literals);
    tcase_add_test(tc_core, test_validate_invalid_structure_missing_comma);
    tcase_add_test(tc_core, test_validate_invalid_structure_missing_colon);
    tcase_add_test(tc_core, test_validate_invalid_unclosed_brackets);
    tcase_add_test(tc_core, test_validate_invalid_extra_tokens_after_value);
    tcase_add_test(tc_core, test_validate_extra_content);
    suite_add_tcase(s, tc_core);

    return s;
}
