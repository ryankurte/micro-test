/**
 * uTest Test Runner (helpers)
 * Designed for embedding tests within an applications with minimal complexity and
 * no external dependencies.
 * @author Ryan Kurte
 */

#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#ifndef UTEST_NO_COLOR

//Colors enabled
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#else

//Colors disabled
#define KNRM  ""
#define KRED  ""
#define KGRN  ""
#define KYEL  ""
#define KBLU  ""
#define KMAG  ""
#define KCYN  ""
#define KWHT  ""

#endif

/***     Module Macros        ***/

//Static variables for communicating outside of tests
#define TEST_MODULE() \
static char test_message[256]; \
static int test_result; \
static char test_file[256]; \
static int test_line_no;

/***     Runner Macros        ***/

//Start a test runner, called before TEST_GROUP and RUN_TEST
#define START_TESTS() \
    int test_fails = 0; \
    int test_success = 0;

//Starts a test group
#define TEST_GROUP(name) \
    printf("\r\nTest group: %s\r\n", name);

//End a test runner, called after all RUN_TEST calls
#define END_TESTS() \
    printf("----------------------------------------\r\n"); \
    printf("Ran %d tests. %d passed, %d failed\r\n", test_fails + test_success, test_success, test_fails); \
    printf("\r\n"); \
    if(test_fails == 0) { \
        return 0; \
    } else { \
        return -1; \
    }

/***     Assertions/check Macros        ***/

//Errors if the value of a is true
#define ASSERT(a) \
    if(a) { \
        test_result = 0; \
        sprintf(test_message, "ASSERT error"); \
        strncpy(test_file, __FILE__, 256); \
        test_line_no = __LINE__; \
        return -1; \
    }
#define CHECK(a) ASSERT(a)

//Errors if a != b
#define CHECK_EQUAL(a, b) \
    if(a != b) { \
        test_result = 0; \
        sprintf(test_message, "CHECK_EQUAL expected: %d actual: %d", a, b); \
        strncpy(test_file, __FILE__, 256); \
        test_line_no = __LINE__; \
        return -1; \
    }

//Errors if called
#define FAIL(res, message) \
    strncpy(test_message, message, 256); \
    strncpy(test_file, __FILE__, 256); \
    test_line_no = __LINE__; \
    test_result = res; \
    return -1;


/***     Test Macros            ***/

//Run a test (with setup and teardown)
//Note that setup, test, and teardown functions must all have the same signature
// ie. int function(context);
#define RUN_TEST(name, function, context) \
    printf(" - %s\t\t", name); \
    if(setup(context) < 0) { \
        printf(KRED "[SETUP FAILED]" KNRM "\r\n"); \
        PRINT_FAIL(); \
    } else { \
        if(function(context) < 0) { \
            printf(KRED "[FAILED]" KNRM "\r\n"); \
            test_fails ++; \
            PRINT_FAIL(); \
            if(teardown(context) < 0) { \
                printf(KRED "[TEARDOWN FAILED]" KNRM "\r\n"); \
                test_fails ++; \
            } \
        } else { \
            if(teardown(context) < 0) { \
                printf(KRED "[TEARDOWN FAILED]" KNRM "\r\n"); \
                test_fails ++; \
            } else { \
                printf(KGRN "[SUCCESS]" KNRM "\r\n"); \
                test_success ++; \
            } \
        }; \
    }

//Run a test only (no setup or teardown)
#define RUN_TEST_ONLY(name, function, context) \
    printf(" - %s\t\t", name); \
    if(function(context) < 0) { \
        printf(KRED "[FAILED]" KNRM "\r\n"); \
        test_fails ++; \
        PRINT_FAIL(); \
    } else { \
        printf(KGRN "[SUCCESS]" KNRM "\r\n"); \
        test_success ++; \
    }

//End a specific test method
#define END_TEST() \
    return 0;

/***     Internal Macros        ***/

//Print cached failure information
#define PRINT_FAIL() \
    printf(KRED "Error: %d '%s' \r\n\tin file: %s:%d\r\n\r\n" KNRM, test_result, test_message, test_file, test_line_no);

#endif
