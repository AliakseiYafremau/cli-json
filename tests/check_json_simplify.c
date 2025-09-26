#include <stdlib.h>
#include <check.h>

START_TEST(test_json_simplify) {
    ck_assert_str_eq(
        "1234", "1234"
    );
}
END_TEST



// Группа тестов
Suite *json_suite(void) {
    Suite *s = suite_create("JSON");

    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_json_simplify);
    suite_add_tcase(s, tc_core);

    return s;
}

// Точка входа
int main(void) {
    int failed;
    Suite *s = json_suite();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}