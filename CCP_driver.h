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
 *   Created on: Oct 9, 2016
 *      Author: Matthias Baumann
 *      email:  baumannmatthias@web.de
 */

#ifndef _CCP_DRIVER_H_
#define _CCP_DRIVER_H_

/*
 * Configure the CCP Driver
 */

#define CCP_STATION_ADDRESS          0		/* $CCP_StationAddress */
#define PROTECTION_MECHANISM	 	 0		/* 0="not active" 1="active" */
#define PROTECT_PROGRAMMING		 	 1		/* 0="not active" 1="active"   Hint: No effect if PROTECTION_MECHANISM = 0 */
#define PROTECT_CALIBRATION 	 	 1		/* 0="not active" 1="active"   Hint: No effect if PROTECTION_MECHANISM = 0 */
#define PROTECT_DATA_AQUISITION	 	 1		/* 0="not active" 1="active"   Hint: No effect if PROTECTION_MECHANISM = 0 */
#define CCP_PROGRAMMING              0		/* 0="not active" 1="active"   Hint: Can not be actived in current version */
#define CCP_DATA_AQUISITION          0		/* 0="not active" 1="active"   Hint: Can not be actived in current version */
#define CCP_CALIBRATION				 1      /* 0="not active" 1="active" */

/*
 * Configure the following Part only if the command EXCHANGE_ID is required
 */

#define LENGTH_OF_SLAVE_ID 			0	   /* Hint: Only positive numbers are allowed */
#define DATATYPE_SLAVE_ID			0      /* Hint: implementation specific, here 0 is used for uint8 */
/*
 * Information about the implemented CCP-Version
 */
#define CCP_VERSION                  2
#define CCP_SUBVERSION               1

/*
 * Defines used inside the Code to avoid numbers
 */

#define NOT_CONNECTED				  0x00
#define CONNECTED					  0x01
#define OFF_LINE					  0x02


#define UNPROTECTED					  0x00
#define PROTECTED					  0x01
#define TEMPORARY_DISCONNECT		  0x00
#define END_OF_SESSION				  0x01
#define BYTE_POSITION_CRC             0x01
#define BYTE_POSITION_PID             0x00
#define BYTE_POSITION_CTR             0x02
#define BYTE_POSITION_VERSION         0x03
#define BYTE_POSITION_SUBVERSION      0x04
#define BYTE_POSITION_DATA            0x03
#define BYTE_POSITION_ADDEXT          0x03
#define BYTE_POSITION_ADDRESS_BYTE1   0x04
#define BYTE_POSITION_ADDRESS_BYTE2   0x05
#define BYTE_POSITION_ADDRESS_BYTE3   0x06
#define BYTE_POSITION_ADDRESS_BYTE4   0x07
#define BYTE_POSITION_LENGTH_ID		  0x03
#define BYTE_POSITION_DATATYPE_ID     0x04
#define BYTE_POSITION_AVAILABLE_MASK  0x05
#define BYTE_POSITION_PROTECTION_MASK 0x06



/*
 * Table of Commands
 */
#define COMMAND_CONNECT               0x01
#define COMMAND_GET_CCP_VERSION       0x1B
#define COMMAND_EXCHANGE_ID           0x17
#define COMMAND_GET_SEED              0x12
#define COMMAND_UNLOCK                0x13
#define COMMAND_SET_MTA               0x02
#define COMMAND_DNLOAD                0x03
#define COMMAND_DNLOAD_6              0x23
#define COMMAND_UPLOAD                0x04
#define COMMAND_SHORT_UP              0x0F
#define COMMAND_SELECT_CAL_PAGE       0x11
#define COMMAND_GET_DAQ_SIZE          0x14
#define COMMAND_SET_DAQ_PTR           0x15
#define COMMAND_WRITE_DAQ             0x16
#define COMMAND_START_STOP            0x06
#define COMMAND_DISCONNECT            0x07
#define COMMAND_SET_S_STATUS          0x0C
#define COMMAND_GET_S_STATUS          0x0D
#define COMMAND_BUILD_CHKSUM          0x0E
#define COMMAND_CLEAR_MEMORY          0x10
#define COMMAND_PROGRAM               0x18
#define COMMAND_PROGRAM_6             0x22
#define COMMAND_MOVE                  0x19
#define COMMAND_TEST                  0x05
#define COMMAND_GET_ACTIVE_CAL_PAGE   0x09
#define COMMAND_START_STOP_ALL        0x08
#define COMMAND_DIAG_SERVICE          0x20
#define COMMAND_ACTION_SERVICE        0x21
/*
 * Table of Command Return Codes
 */
#define CRC_ACKNOWLEGE               0x00
#define CRC_DAQ_PRCESSOR_OVERLOAD    0x01
#define CRC_COMMAND_PROCESSOR_BUSY   0x10
#define CRC_DAQ_PROCESSOR_BUSY       0x11
#define CRC_INTERNAL_TIMEOUT         0x12
#define CRC_KEY_REQUEST              0x18
#define CRC_SESSION_STATUS_REQUEST   0x19
#define CRC_COLD_START_REQUEST       0x20
#define CRC_CAL_DATA_INIT_REQUEST    0x21
#define CRC_DAQ_LIST_INIT_REQUEST    0x22
#define CRC_CODE_UPDATE_REQUEST      0x23
#define CRC_UNKNOWN_COMMAND          0x30
#define CRC_COMMAND_SYNTAX           0x31
#define CRC_PARAMETER_OUT_OF_RANGE   0x32
#define CRC_ACCESS_DENIED            0x33
#define CRC_OVERLOAD                 0x34
#define CRC_ACCESS_LOCKED            0x35
#define CRC_RESOURCE_FUNC_NOT_AVAIL  0x36

void CCP_driver_init(void);
void CRO_analyse(uint8_t * ptr_first_byte_cmd);
void send_DTO(uint8_t * ptr_first_byte_cmd);
void callback_CONNECT(uint8_t * ptr_first_byte_cmd);
void callback_DISCONNECT(uint8_t * ptr_first_byte_cmd);
void callback_GET_CCP_VERSION(uint8_t * ptr_first_byte_cmd);
void callback_EXCHANGE_ID(uint8_t * ptr_first_byte_cmd);
void callback_SET_MTA(uint8_t * ptr_first_byte_cmd);
void callback_UPLOAD(uint8_t * ptr_first_byte_cmd);
void callback_DNLOAD(uint8_t * ptr_first_byte_cmd);
void callback_SHORT_UP(uint8_t * ptr_first_byte_cmd);

#endif /* 44_CCP_CCP_DRIVER_H_ */
