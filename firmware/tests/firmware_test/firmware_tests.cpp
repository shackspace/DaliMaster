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

    EXPECT_EQ(_ERR_OK_, dali_special_command(&temp, PROGRAM_SHORT_ADDRESS, 0x01));

    EXPECT_EQ(0xB703, temp);
}

TEST(ParseInt, POSITIVE)
{
    int16_t i = 0;

    EXPECT_EQ(_ERR_OK_, parse_int("-123",&i));

    EXPECT_EQ(-123,i);

    EXPECT_EQ(_ERR_OK_, parse_int("123",&i));

    EXPECT_EQ(123,i);

    EXPECT_EQ(_ERR_OK_, parse_int("-1234",&i));

    EXPECT_EQ(-1234,i);

    EXPECT_EQ(_ERR_OK_, parse_int("1234",&i));

    EXPECT_EQ(1234,i);

    EXPECT_EQ(_ERR_PARSE_ERROR_, parse_int("1234\n",&i));
}

TEST(ParserTestValidInputString, POSITIVE)
{
    unsigned short frame;
    unsigned short frame2;

    EXPECT_EQ(_MODE_SIMPLE_, decode_command_to_frame("up 1\n", &frame));

    dali_slave_command(&frame2, 0x01, DALI_UP_200MS);

    EXPECT_EQ(frame2, frame);

    EXPECT_EQ(_MODE_SIMPLE_, decode_command_to_frame("up_g 1\n", &frame));

    dali_group_command(&frame2, 0x01, DALI_UP_200MS);

    EXPECT_EQ(frame2, frame);

    EXPECT_EQ(_MODE_SIMPLE_, decode_command_to_frame("arc 1 8\n", &frame));

    dali_slave_direct_arc(&frame2, 0x01, 0x08);

    EXPECT_EQ(frame2, frame);

    EXPECT_EQ(_MODE_SIMPLE_, decode_command_to_frame("arc_g 1 8\n", &frame));

    dali_group_direct_arc(&frame2, 0x01, 0x08);

    EXPECT_EQ(frame2, frame);

    EXPECT_EQ(_MODE_REPEAT_TWICE_, decode_command_to_frame("randomize\n", &frame));

    dali_command_randomize(&frame2  );

    EXPECT_EQ(frame2, frame);

    EXPECT_EQ(_MODE_REPEAT_TWICE_, decode_command_to_frame("initialize 255\n", &frame));

    dali_command_initialize_broadcast(&frame2);

    EXPECT_EQ(frame2, frame);

    EXPECT_EQ(_MODE_REPEAT_TWICE_, decode_command_to_frame("initialize 255 55 55 55 jkhsdfb\n", &frame)); //overlength of frame, but possible

    dali_command_initialize_broadcast(&frame2);

    EXPECT_EQ(frame2, frame);
}

TEST(ParserTestInvalidInputString, POSITIVE)
{
    unsigned short frame;
    unsigned short frame2;

    EXPECT_EQ(_ERR_PARSE_ERROR_, decode_command_to_frame("up 1000\n", &frame));

    EXPECT_EQ(_ERR_UNIMPLEMENTED_, decode_command_to_frame("updown 10 39\n", &frame));
}


int main(int argv, char** argc)
{
	::testing::InitGoogleTest(&argv, argc);
	return RUN_ALL_TESTS();
}

