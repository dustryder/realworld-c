#include "unity.h"
#include "article_service_tests.h"
#include "lib_tests.h"

void setUp(void) {
}

void tearDown(void) {
}

int main(void) {
    UNITY_BEGIN();

    run_article_service_tests();
    run_lib_tests();

    return UNITY_END();
}