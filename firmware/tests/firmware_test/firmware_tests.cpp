#include "../../Interpreter.h"

#include <limits.h>
#include <gtest/gtest.h>

TEST(TestEncoderDirectLevelControl,POSITIVE)
{
	word temp;
	
	dali_slave_direct_arc(&temp, 0x3F, 0x7F); //must be valid dali frame
	
	EXPECT_EQ(0x7E7F, temp);

	EXPECT_EQ(_ERR_WRONG_ADDRESS_, dali_slave_direct_arc(&temp, 64, 0)); //address out scope

	dali_group_direct_arc(&temp, 0x0F, 0x7F); //must be valid dali frame
	
	EXPECT_EQ(0x9E7F, temp);

	EXPECT_EQ(_ERR_WRONG_ADDRESS_, dali_group_direct_arc(&temp, 16, 0)); //address out scope

}

TEST(TestEncoderCommands,POSITIVE)
{
	word temp;
	
	dali_slave_command(&temp, 0x3F, 0x7F);
	
	EXPECT_EQ(0x7F7F, temp);

	EXPECT_EQ(_ERR_WRONG_ADDRESS_, dali_slave_command(&temp, 64, 0));

	dali_group_command(&temp, 0x3F, 0x7F);
	
	EXPECT_EQ(0x7F7F, temp);

	EXPECT_EQ(_ERR_WRONG_ADDRESS_, dali_group_command(&temp, 64, 0));
}

TEST(TestEncoderSpecialCommands,POSITIVE)
{
	word temp;
	
	EXPECT_EQ(_ERR_OK_, dali_special_command(&temp, INITIALIZE, 0));
	
	EXPECT_EQ(0xA500, temp);
}

int main(int argv, char** argc)
{
	::testing::InitGoogleTest(&argv, argc);
	return RUN_ALL_TESTS();
}

