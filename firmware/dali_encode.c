#include "dali_encode.h"

inline int dali_slave_direct_arc(word *output, byte address, byte brightness)
{
	if(address > 63)
	{
		return _ERR_WRONG_ADDRESS_;
	}
	*output = (0x80 + (address << 1)) << 8 + brightness; //Frame: 1AAAAAA0 BBBBBBBB
	return 0;
}

inline int dali_slave_commad(word *output, byte address, byte command)
{
	if(address > 63)
		return _ERR_WRONG_ADDRESS_;
	if(!((command & 0xF0) ^ 0x30)) //command 0011xxxx is reserved
		return _ERR_RESERVED_COMMAND_;
	if(!((command & 0xF0) ^ 0x80)) //command 1000xxxx is reserved
		return _ERR_RESERVE_COMMAND_;
	if(!((command & 0xFC) ^ 0x9C)) //command 100111xx is reserved
		return _ERR_RESERVED_COMMAND_;
	if(!((command & 0xF0) ^ 0xA0)) //command 1010xxxx is reserved
		return _ERR_RESERVE_COMMAND_;
	if(!((command & 0xE0) ^ 0xC0)) //command 110xxxxx is reserved
		return _ERR_RESERVE_COMMAND_;
	*output = (0x81 + (address << 1)) << 8 + command;  //Frame: 1AAAAAA1 CCCCCCCC
	return 0;
}

inline int dali_special_command(word *output, special_commands command, byte data)
{	
	return 0;
}

