#include <stdio.h>
#include <string.h>

#include "micro_test.h"

static int fake_test_pass(void* context);
static int fake_test_fail(void* context);

TEST_MODULE();

int setup(void* context) {
	//Per test setup here
	return 0;
}

int teardown(void* context) {
	//Per test teardown here
	return 0;
}

int main(int argc, char** argv) 
{
  START_TESTS();

  TEST_GROUP("Fake Tests");
  
  RUN_TEST("Fake Pass Test", fake_test_pass, NULL);

  RUN_TEST("Fake Fail Test", fake_test_fail, NULL);

  END_TESTS();
}

static int fake_test_pass(void* context)
{
  CHECK_EQUAL(0, 0);
  
  ASSERT(0);
  
  END_TEST();
}

static int fake_test_fail(void* context)
{
  CHECK_EQUAL(0, 0);
  
  ASSERT(0);
  
  FAIL(0, "Error Here");
  
  END_TEST();
}