#include <stdio.h>
#include <string.h>

#include "micro_test.h"

static int fake_test(void* context);

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

  TEST_GROUP("Fake");
  
  RUN_TEST("Fake Test", fake_test, NULL);

  END_TESTS();
}

static int fake_test(void* context)
{
  CHECK_EQUAL(0, 0);
  
  ASSERT(0);
  
  FAIL(0, "Error Here");
  
  END_TEST();
}