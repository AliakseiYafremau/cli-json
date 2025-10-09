#include <check.h>
#include <stdlib.h>

#include "check_simplify_json.h"

int main(void) {
    int failed;
    Suite *s = json_suite();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}