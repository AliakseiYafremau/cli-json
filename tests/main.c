#include <check.h>
#include <stdlib.h>

#include "check_format_json.h"
#include "check_simplify_json.h"
#include "check_validate_json.h"

int main(void) {
    int failed;
    Suite* simplify_suite = json_simplify_suite();
    Suite* format_suite = json_format_suite();
    Suite* validate_suite = json_validate_suite();
    SRunner* sr = srunner_create(simplify_suite);
    srunner_add_suite(sr, format_suite);
    srunner_add_suite(sr, validate_suite);

    srunner_run_all(sr, CK_NORMAL);
    failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
