/*
 *   This CCP driver is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This CCP driver is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with CCP driver.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Created on: Oct 9, 2016
 *      Author: Matthias Baumann
 */

#include <inttypes.h>
#include "CCP_driver.h"


static struct CCP_connection
{
	uint8_t connection_status;
	uint8_t * ptr_MTA_0;
	uint8_t  MTA0_extention;
	uint8_t * ptr_MTA_1;
	uint8_t  MTA1_extention;
	uint16_t station_address;
} CCP_status;


void CCP_driver_init(void)
{
	CCP_status.connection_status = NOT_CONNECTED;
	CCP_status.ptr_MTA_0 = 0;
	CCP_status.ptr_MTA_1 = 0;
	CCP_status.MTA0_extention =0;
	CCP_status.MTA1_extention =0;
	CCP_status.station_address = STATION_ADDRESS;
}

void CRO_analyse(uint8_t * ptr_first_byte_cmd)
{
	uint8_t  *ptr_CRO_command = ptr_first_byte_cmd;  // byte 1 Command Counter;

	//Check which command had been requested
	switch (*ptr_CRO_command) {
		case COMMAND_CONNECT:
			callback_CONNECT(ptr_first_byte_cmd);
			break;
		case COMMAND_SET_MTA:
			callback_SET_MTA(ptr_first_byte_cmd);
			break;
		case COMMAND_GET_CCP_VERSION:
			callback_GET_CCP_VERSION(ptr_first_byte_cmd);
			break;
		case COMMAND_UPLOAD:
			callback_UPLOAD(ptr_first_byte_cmd);
			break;
		case COMMAND_DNLOAD:
			callback_DNLOAD(ptr_first_byte_cmd);
			break;
		default:

			break;
	}
}


void callback_CONNECT(uint8_t * ptr_first_byte_cmd)
{
	uint8_t answer[8];

	uint8_t  *ptr_CRO_ctr = ptr_first_byte_cmd + 1;  // byte 2 Command Counter

	uint16_t CRO_station_address;
	#if CONTROLER_ENDIANNESS == LITTLE_ENDIANNESS
		CRO_station_address = *((uint16_t * )( ptr_first_byte_cmd + 2 ));  // byte 3 and 4
	#endif
	#if CONTROLER_ENDIANNESS == BIG_ENDIANNESS
		CRO_station_address =  (*((uint16_t *)(ptr_first_byte_cmd + 2))) << 2;  // byte 3 and 4
		CRO_station_address = CRO_station_address + (uint16_t)(*(ptr_first_byte_cmd + 3));  // byte 3 and 4
	#endif

	// Check if the station ID is the correct one
	if(CRO_station_address == STATION_ADDRESS)
	{
		CCP_status.connection_status = CONNECTED;
		answer[BYTE_POSITION_CRC] = CRC_ACKNOWLEGE;
	}
	else
	{
		CCP_status.connection_status = NOT_CONNECTED;
		answer[BYTE_POSITION_CRC] = CRC_PARAMETER_OUT_OF_RANGE;
	}
	answer[BYTE_POSITION_PID] = 0xFF;
	answer[BYTE_POSITION_CTR] = *ptr_CRO_ctr;

	send_DTO(answer);
}


void callback_GET_CCP_VERSION(uint8_t * ptr_first_byte_cmd)
{
	uint8_t answer[8];

	uint8_t *ptr_CRO_ctr              = ptr_first_byte_cmd + 1;  // byte 2 Command Counter
	uint8_t *ptr_CRO_CCP_version      = ptr_first_byte_cmd + 2;  // byte 3
	uint8_t *ptr_CRO_CCP_subversion   = ptr_first_byte_cmd + 3;  // byte 3

	// Check if the station ID is the correct one
	if(CCP_status.connection_status == CONNECTED)
	{
		answer[BYTE_POSITION_PID] = 0xFF;
		answer[BYTE_POSITION_CRC] = CRC_ACKNOWLEGE;
		answer[BYTE_POSITION_CTR] = *ptr_CRO_ctr;
		answer[BYTE_POSITION_VERSION] = CCP_VERSION;
		answer[BYTE_POSITION_SUBVERSION] = CCP_SUBVERSION;
		send_DTO(answer);
	}
}

void callback_SET_MTA(uint8_t * ptr_first_byte_cmd)
{
	uint8_t answer[8]={0,0,0,0,0,0,0,0};

	uint8_t *ptr_CRO_ctr              = ptr_first_byte_cmd + 1;  // byte 2 Command Counter
	uint8_t *ptr_CRO_MTA_number       = ptr_first_byte_cmd + 2;  // byte 3 Memory Transfer Address number
	uint8_t *ptr_CRO_MTA_extention    = ptr_first_byte_cmd + 3;  // byte 3 Memory Extention

	uint8_t array_address_bytes[4];
	array_address_bytes[0] = *(ptr_first_byte_cmd + 4);
	array_address_bytes[1] = *(ptr_first_byte_cmd + 5);
	array_address_bytes[2] = *(ptr_first_byte_cmd + 6);
	array_address_bytes[3] = *(ptr_first_byte_cmd + 7);

	uint32_t *ptr_CRO_address 		  = (uint32_t * )( array_address_bytes );


	// Check if the station ID is the correct one
	if(CCP_status.connection_status == CONNECTED)
	{
		switch (*ptr_CRO_MTA_number)
		{
			case 0:
				answer[BYTE_POSITION_CRC] = CRC_ACKNOWLEGE;
				CCP_status.ptr_MTA_0 = *ptr_CRO_address;
				CCP_status.MTA0_extention = *ptr_CRO_MTA_extention;
				break;
			case 1:
				answer[BYTE_POSITION_CRC] = CRC_ACKNOWLEGE;
				CCP_status.ptr_MTA_1 = *ptr_CRO_address;
				CCP_status.MTA1_extention = *ptr_CRO_MTA_extention;
				break;
			default:
				answer[BYTE_POSITION_CRC] = CRC_PARAMETER_OUT_OF_RANGE;
				break;
		}

		answer[BYTE_POSITION_PID] = 0xFF;
		answer[BYTE_POSITION_CTR] = *ptr_CRO_ctr;

	}
	else
	{
		answer[BYTE_POSITION_PID] = 0xFF;
		answer[BYTE_POSITION_CTR] = CRC_ACCESS_DENIED;

	}
	send_DTO(answer);
}


void callback_UPLOAD(uint8_t * ptr_first_byte_cmd)
{
	uint8_t answer[8];

	uint8_t *ptr_CRO_ctr              = ptr_first_byte_cmd + 1;  // byte 2 Command Counter
	uint8_t *ptr_CRO_upload_size      = ptr_first_byte_cmd + 2;  // byte 3 Number of Bytes that shall be uploaded

	// Check if the station ID is the correct one
	if(CCP_status.connection_status == CONNECTED)
	{
		uint8_t i;
		if (*ptr_CRO_upload_size > 0 && *ptr_CRO_upload_size < 5)
		{
			answer[BYTE_POSITION_CRC] = CRC_ACKNOWLEGE;
			for(i = 0; i < *ptr_CRO_upload_size;i++ )
			{
				*(answer + BYTE_POSITION_DATA + i) = *(CCP_status.ptr_MTA_0++);
			}
		}
		else
		{
			answer[BYTE_POSITION_CRC] = CRC_PARAMETER_OUT_OF_RANGE;
		}
		answer[BYTE_POSITION_PID] = 0xFF;
		answer[BYTE_POSITION_CTR] = *ptr_CRO_ctr;
		send_DTO(answer);
	}
}

void callback_DNLOAD(uint8_t * ptr_first_byte_cmd)
{
	uint8_t answer[8];

	uint8_t *ptr_CRO_ctr              = ptr_first_byte_cmd + 1;  // byte 2 Command Counter
	uint8_t *ptr_CRO_dnload_size      = ptr_first_byte_cmd + 2;  // byte 3 Number of Bytes that shall be uploaded

	// Check if the station ID is the correct one
	if(CCP_status.connection_status == CONNECTED)
	{
		uint8_t i;
		if (*ptr_CRO_dnload_size > 0 && *ptr_CRO_dnload_size < 5)
		{
			answer[BYTE_POSITION_CRC] = CRC_ACKNOWLEGE;
			for(i = 0; i < *ptr_CRO_dnload_size;i++ )
			{
				*(CCP_status.ptr_MTA_0++) = *(ptr_first_byte_cmd + 3 + i);
			}
			uint8_t *ptr_MTA0;
			ptr_MTA0 = CCP_status.ptr_MTA_0;
			uint32_t MTA0_Adress = &ptr_MTA0;
			answer[BYTE_POSITION_ADDRESS_BYTE1] = (uint8_t)((MTA0_Adress & 0xFF000000) >> 24);
			answer[BYTE_POSITION_ADDRESS_BYTE2] = (uint8_t)((MTA0_Adress & 0x00FF0000) >> 16);
			answer[BYTE_POSITION_ADDRESS_BYTE3] = (uint8_t)((MTA0_Adress & 0x0000FF00) >> 8);
			answer[BYTE_POSITION_ADDRESS_BYTE4] = (uint8_t)((MTA0_Adress & 0x000000FF) );
			answer[BYTE_POSITION_ADDEXT]  = CCP_status.MTA0_extention;
		}
		else
		{
			answer[BYTE_POSITION_CRC] = CRC_PARAMETER_OUT_OF_RANGE;
		}
		answer[BYTE_POSITION_PID] = 0xFF;
		answer[BYTE_POSITION_CTR] = *ptr_CRO_ctr;
		send_DTO(answer);
	}
}

/*
 * The following function shall push the data to the interface that shall be used for communication.
 *
 * Best practice would be to copy the prototype below in a new C-File (e.g. CCP_integration.c) and
 * define the function in there. By this the driver could be quiet easily be updated. If a new version
 * will be released, just replace the two files CCP_driver.c/h. The integration will stay the same
 * as it is part of the target specific integration file CCP_integration.c.
 *

	void send_DTO(uint8_t * ptr_first_byte_cmd)
	{
		uint8_t i;
		uint8_t *ptr_tmp =ptr_first_byte_cmd;
		out_char(0xB0);
		for(i= 0; i<8; i++)
		{
			// Push the data to the CAN or UART driver
		}
	}

*/

