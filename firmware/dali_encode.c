#include "dali_encode.h"

int dali_slave_direct_arc(word *output, byte address, byte brightness)
{
	if(address > 63)
	{
		return _ERR_WRONG_ADDRESS_;
	}

	brightness = brightness > 254 ? 254 : brightness; //direct arc value can be 254 maximum
	
	*output = ((address << 1) << 8) + brightness; //Frame: 0AAAAAA0 BBBBBBBB
	return _ERR_OK_;
}

int dali_group_direct_arc(word *output, byte address, byte brightness)
{
	if(address > 15)
	{
		return _ERR_WRONG_ADDRESS_;
	}

	brightness = brightness > 254 ? 254 : brightness; //direct arc value can be 254 maximum
	
	*output = ((0x80 + (address << 1)) << 8) + brightness; //Frame: 100AAAA0 BBBBBBBB
	return _ERR_OK_;
}

int dali_broadcast_direct_arc(word *output, byte brightness)
{
	brightness = brightness > 254 ? 254 : brightness; //direct arc value can be 254 maximum
	
	*output = 0xFE00 + brightness; //Frame: 11111110 BBBBBBBB
	return _ERR_OK_;
}

int dali_slave_command(word *output, byte address, byte command)
{
	if(address > 63)
		return _ERR_WRONG_ADDRESS_;
	if(!((command & 0xF0) ^ 0x30)) //command 0011xxxx is reserved
		return _ERR_RESERVED_COMMAND_;
	if(!((command & 0xF0) ^ 0x80)) //command 1000xxxx is reserved
		return _ERR_RESERVED_COMMAND_;
	if(!((command & 0xFC) ^ 0x9C)) //command 100111xx is reserved
		return _ERR_RESERVED_COMMAND_;
	if(!((command & 0xF0) ^ 0xA0)) //command 1010xxxx is reserved
		return _ERR_RESERVED_COMMAND_;
	if(!((command & 0xE0) ^ 0xC0)) //command 110xxxxx is reserved
		return _ERR_RESERVED_COMMAND_;

	if((command == DALI_GO_TO_SCENE) || 
	(command == DALI_STORE_THE_DTR_AS_SCENE) || 
	(command == DALI_REMOVE_FROM_SCENE) ||
	(command == DALI_ADD_TO_GROUP) ||
	(command == DALI_REMOVE_FROM_GROUP) ||
	(command == DALI_QUERY_SCENE_LEVEL))
		return _ERR_WRONG_COMMAND_;
	
	*output = ((0x01 + (address << 1)) << 8) + command;  //Frame: 0AAAAAA1 CCCCCCCC
	return _ERR_OK_;
}

int dali_group_command(word *output, byte address, byte command)
{
	if(address > 15)
		return _ERR_WRONG_ADDRESS_;
	if(!((command & 0xF0) ^ 0x30)) //command 0011xxxx is reserved
		return _ERR_RESERVED_COMMAND_;
	if(!((command & 0xF0) ^ 0x80)) //command 1000xxxx is reserved
		return _ERR_RESERVED_COMMAND_;
	if(!((command & 0xFC) ^ 0x9C)) //command 100111xx is reserved
		return _ERR_RESERVED_COMMAND_;
	if(!((command & 0xF0) ^ 0xA0)) //command 1010xxxx is reserved
		return _ERR_RESERVED_COMMAND_;
	if(!((command & 0xE0) ^ 0xC0)) //command 110xxxxx is reserved
		return _ERR_RESERVED_COMMAND_;

	if((command == DALI_GO_TO_SCENE) || 
	(command == DALI_STORE_THE_DTR_AS_SCENE) || 
	(command == DALI_REMOVE_FROM_SCENE) ||
	(command == DALI_ADD_TO_GROUP) ||
	(command == DALI_REMOVE_FROM_GROUP) ||
	(command == DALI_QUERY_SCENE_LEVEL))
		return _ERR_WRONG_COMMAND_;
	
	*output = ((0x81 + (address << 1)) << 8) + command;  //Frame: 100AAAA1 CCCCCCCC
	return _ERR_OK_;
}

int dali_broadcast_command(word *output, byte command)
{
	if(!((command & 0xF0) ^ 0x30)) //command 0011xxxx is reserved
		return _ERR_RESERVED_COMMAND_;
	if(!((command & 0xF0) ^ 0x80)) //command 1000xxxx is reserved
		return _ERR_RESERVED_COMMAND_;
	if(!((command & 0xFC) ^ 0x9C)) //command 100111xx is reserved
		return _ERR_RESERVED_COMMAND_;
	if(!((command & 0xF0) ^ 0xA0)) //command 1010xxxx is reserved
		return _ERR_RESERVED_COMMAND_;
	if(!((command & 0xE0) ^ 0xC0)) //command 110xxxxx is reserved
		return _ERR_RESERVED_COMMAND_;

	if((command == DALI_GO_TO_SCENE) || 
	(command == DALI_STORE_THE_DTR_AS_SCENE) || 
	(command == DALI_REMOVE_FROM_SCENE) ||
	(command == DALI_ADD_TO_GROUP) ||
	(command == DALI_REMOVE_FROM_GROUP) ||
	(command == DALI_QUERY_SCENE_LEVEL))
		return _ERR_WRONG_COMMAND_;
	
	*output = 0xFF00 + command;  //Frame: 11111111 CCCCCCCC
	return _ERR_OK_;
}


int dali_slave_command_with_param(word *output, byte address, byte command, byte param)
{
	if(address > 63)
		return _ERR_WRONG_ADDRESS_;
	if(param > 15)
		return _ERR_WRONG_COMMAND_;	

	if((command == DALI_GO_TO_SCENE) || 
	(command == DALI_STORE_THE_DTR_AS_SCENE) || 
	(command == DALI_REMOVE_FROM_SCENE) ||
	(command == DALI_ADD_TO_GROUP) ||
	(command == DALI_REMOVE_FROM_GROUP) ||
	(command == DALI_QUERY_SCENE_LEVEL))
	{
		*output = ((0x01 + (address << 1)) << 8) + command + param;  //Frame: 0AAAAAA1 CCCCPPPP
		return _ERR_OK_;
	}
	else
		return _ERR_WRONG_COMMAND_;
}

int dali_group_command_with_param(word *output, byte address, byte command, byte param)
{
	if(address > 15)
		return _ERR_WRONG_ADDRESS_;
	if(param > 15)
		return _ERR_WRONG_COMMAND_;
		
	if((command == DALI_GO_TO_SCENE) || 
	(command == DALI_STORE_THE_DTR_AS_SCENE) || 
	(command == DALI_REMOVE_FROM_SCENE) ||
	(command == DALI_ADD_TO_GROUP) ||
	(command == DALI_REMOVE_FROM_GROUP) ||
	(command == DALI_QUERY_SCENE_LEVEL))
	{
		*output = ((0x81 + (address << 1)) << 8) + command + param;  //Frame: 100AAAA1 CCCCPPPP
		return _ERR_OK_;
	}
	else
		return _ERR_WRONG_COMMAND_;
}

int dali_broadcast_command_with_param(word *output, byte command, byte param)
{
	if(param > 15)
		return _ERR_WRONG_COMMAND_;
		
	if((command == DALI_GO_TO_SCENE) || 
	(command == DALI_STORE_THE_DTR_AS_SCENE) || 
	(command == DALI_REMOVE_FROM_SCENE) ||
	(command == DALI_ADD_TO_GROUP) ||
	(command == DALI_REMOVE_FROM_GROUP) ||
	(command == DALI_QUERY_SCENE_LEVEL))
	{
		*output = 0xFF00 + command + param;  //Frame: 100AAAA1 CCCCPPPP
		return _ERR_OK_;
	}
	else
		return _ERR_WRONG_COMMAND_;
}

int dali_special_command(word *output, special_command_type command, byte data)
{	
	switch(command)
	{
	case INITIALIZE:
		*output = (DALI_INITIALISE << 8) + data;  //Frame: 1010 0101 xxxx xxxx
		break;
	case RANDOMIZE:
		*output = DALI_RANDOMISE << 8;   //1010 0111 0000 0000 
		break;
	case TERMINATE:
		*output = DALI_TERMINATE << 8;   //1010 0001 0000 0000 
		break;
	case STORE_DTR:
		*output = (DALI_DATA_TRANSFER_REGISTER << 8) + data;
		break;
	case PROGRAM_SHORT_ADDRESS:
		*output = (DALI_PROGRAM_SHORT_ADDRESS << 8) + 0x01 + ((0x3F & data) << 1); //1011 0111 0xxx xxx1 
		break;
	case VERIFY_SHORT_ADDRESS:
		*output = (DALI_VERIFY_SHORT_ADDRESS << 8) + 0x01 + ((0x3F & data) << 1);
		break;
	case QUERY_SHORT_ADDRESS:
		*output = DALI_QUERY_SHORT_ADDRESS << 8;
		break;
	case COMPARE:
		*output = DALI_COMPARE << 8;
		break;
	case WITHDRAW:
		*output = DALI_WITHDRAW << 8;
		break;
	case SEARCH_ADDRESS_H:
		*output = (DALI_SEARCHADDRH << 8) + data;
		break;
	case SEARCH_ADDRESS_M:
		*output = (DALI_SEARCHADDRM << 8) + data;
		break;
	case SEARCH_ADDRESS_L:
		*output = (DALI_SEARCHADDRL << 8) + data;
		break;
	case PHYSICAL_SELECTION:
		*output = (DALI_PHYSICAL_SELECTION << 8);
		break;
	case ENABLE_DEVICE_TYPE:
		*output = (DALI_ENABLE_DEVICE_TYPE_X << 8) + data;
		break;
	default:
		return _ERR_WRONG_COMMAND_;
		break;
	}	
	return _ERR_OK_;
}

