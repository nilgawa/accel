#include "accel.core.h"
#include "gtest/gtest.h"

TEST(CoreResult, Success) {
	int array[] = { 0,1,2,3,4,5,6,7,8,9 };

	auto wrapper = accel::input_wrapper{ array };
	auto result = accel::success<int>(wrapper, 0);
	ASSERT_TRUE(accel::is_success(result));
}
