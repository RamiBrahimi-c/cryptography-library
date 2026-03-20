#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Test runner macros
#define TEST(name) void test_##name()
#define RUN_TEST(name) do { \
    printf("Running %s... ", #name); \
    if (test_##name() == 0) \
        printf("PASSED\n"); \
    else \
        printf("FAILED\n"); \
} while(0)

// Assertions
#define ASSERT_EQ(actual, expected) \
    if ((actual) != (expected)) { \
        printf("\n  ASSERT_EQ failed: %d != %d (line %d)\n", \
               (actual), (expected), __LINE__); \
        return 1; \
    }

#define ASSERT_STR_EQ(actual, expected) \
    if (strcmp(actual, expected) != 0) { \
        printf("\n  ASSERT_STR_EQ failed: \"%s\" != \"%s\" (line %d)\n", \
               (actual), (expected), __LINE__); \
        return 1; \
    }

#define ASSERT_TRUE(condition) \
    if (!(condition)) { \
        printf("\n  ASSERT_TRUE failed (line %d)\n", __LINE__); \
        return 1; \
    }

#define ASSERT_NOT_NULL(ptr) \
    if ((ptr) == NULL) { \
        printf("\n  ASSERT_NOT_NULL failed (line %d)\n", __LINE__); \
        return 1; \
    }

// Test colors (optional)
#define COLOR_GREEN "\033[0;32m"
#define COLOR_RED "\033[0;31m"
#define COLOR_RESET "\033[0m"

// Test summary tracker
typedef struct {
    int passed;
    int failed;
} TestResult;

#define TEST_START() TestResult result = {0, 0}
#define TEST_PASS() result.passed++
#define TEST_FAIL() result.failed++
#define TEST_SUMMARY() \
    printf("\nSummary: %d passed, %d failed\n", result.passed, result.failed)

#endif