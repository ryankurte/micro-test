/**
 * uTest Test Runner (helpers)
 * Designed for embedding tests within an applications with minimal complexity and
 * no external dependencies.
 * @author Ryan Kurte
 */

#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <inttypes.h>

//Enable or disable colours
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

//Enable or disable test timing
#ifndef UTEST_NO_TIMING

//Timing enabled

#include <time.h>

//Optional clocks per second to bypass default for embedded platforms
#ifndef UTEST_CLOCKS_PER_SEC
#define UTEST_CLOCKS_PER_SEC CLOCKS_PER_SEC
#endif

#define SETUP_TIMING() uint32_t start, test_time = 0, total_time = 0;
#define START_TIMING() start=clock();
#define END_TIMING() test_time = (clock() - start) * 1000 / UTEST_CLOCKS_PER_SEC; total_time += test_time;

#define PRINT_SUCCESS() printf(KGRN "[SUCCESS] (%" PRIu32 " ms)" KNRM "\r\n", test_time);
#define PRINT_FAILURE() printf(KRED "[FAILURE] (%" PRIu32 " ms)" KNRM "\r\n", test_time);
#define PRINT_END() printf("Ran %d tests. %d passed, %d failed in %" PRIu32 " ms\r\n", \
                           test_fails + test_success, test_success, test_fails, total_time)

#else

//Timing Disabled

#define SETUP_TIMING()
#define START_TIMING()
#define END_TIMING()

#define PRINT_SUCCESS() printf(KGRN "[SUCCESS]" KNRM "\r\n")
#define PRINT_FAILURE() printf(KRED "[FAILURE]" KNRM "\r\n")
#define PRINT_END() printf("Ran %d tests. %d passed, %d failed\r\n", \
                           test_fails + test_success, test_success, test_fails)

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
    int test_fails = 0, test_success = 0, test_res = 0; \
    SETUP_TIMING();

//Starts a test group
#define TEST_GROUP(name) \
    printf("\r\nTest group: %s\r\n", name);

//End a test runner, called after all RUN_TEST calls
#define END_TESTS() \
    printf("----------------------------------------\r\n"); \
    PRINT_END(); \
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
#define RUN_TEST(name, function, ...) \
    printf(" - %s", name); \
    if(setup(__VA_ARGS__) < 0) { \
        printf(KRED "[SETUP FAILED]" KNRM "\r\n"); \
        PRINT_FAIL(); \
    } else { \
        START_TIMING(); \
        test_res = function(__VA_ARGS__); \
        END_TIMING(); \
        if(test_res < 0) { \
            PRINT_FAILURE() \
            test_fails ++; \
            PRINT_FAIL(); \
            if(teardown(__VA_ARGS__) < 0) { \
                printf(KRED "[TEARDOWN FAILED]" KNRM "\r\n"); \
                test_fails ++; \
            } \
        } else { \
            if(teardown(__VA_ARGS__) < 0) { \
                printf(KRED "[TEARDOWN FAILED]" KNRM "\r\n"); \
                test_fails ++; \
            } else { \
                PRINT_SUCCESS(); \
                test_success ++; \
            } \
        }; \
    }

//Run a test only (no setup or teardown)
#define RUN_TEST_ONLY(name, function, ...) \
    printf(" - %s", name); \
    START_TIMING(); \
    test_res = function(__VA_ARGS__); \
    END_TIMING(); \
    if(test_res < 0) { \
        PRINT_FAILURE(); \
        test_fails ++; \
        PRINT_FAIL(); \
    } else { \
        PRINT_SUCCESS(); \
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

