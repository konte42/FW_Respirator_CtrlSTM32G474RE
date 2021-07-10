/*
 * XCP.h
 *
 *  Created on: 27 Mar 2021
 *      Author: Azi
 */

#ifndef INC_XCP_H_
#define INC_XCP_H_

//definitions for actual addresses
#define Motorcontrol_Mode               0x5080A800
#define Motorcontrol_Mode_S             0x40001904  //in extension = 1

#define MTC_Switch                      0x50808698
#define MTC_S_Switch                    0x40001908  //in extension = 1

#define MC_DebugTorque_Nm               0x5080c4b8
#define MC_S_DebugTorque_Nm             0x40002180  //in extension = 1
#define RequestedTorque                 0x4001fe04

#define Pinion_angle                    0x508089c8

#define inExtension0                    0
#define inExtension1                    1

#define SLAVE_ID                        0x63D
#define MASTER_ID                       0x63C

#define XCP_ERR_CMD_SYNCH                            0x00
#define XCP_ERR_CMD_BUSY                             0x10
#define XCP_ERR_DAQ_ACTIVE                           0x11
#define XCP_ERR_PGM_ACTIVE                           0x12
#define XCP_ERR_CMD_UNKNOWN                          0x20 /** Unknown command or not implemented optional command */
#define XCP_ERR_CMD_SYNTAX                           0x21
#define XCP_ERR_OUT_OF_RANGE                         0x22 /** Command syntax valid but command parameter(s) out of range */
#define XCP_ERR_WRITE_PROTECTED                      0x23
#define XCP_ERR_ACCESS_DENIED                        0x24
#define XCP_ERR_ACCESS_LOCKED                        0x25
#define XCP_ERR_PAGE_NOT_VALID                       0x26
#define XCP_ERR_MODE_NOT_VALID                       0x27
#define XCP_ERR_SEGMENT_NOT_VALID                    0x28
#define XCP_ERR_SEQUENCE                             0x29
#define XCP_ERR_DAQ_CONFIG                           0x2A
#define XCP_ERR_MEMORY_OVERFLOW                      0x30
#define XCP_ERR_GENERIC                              0x31
#define XCP_ERR_VERIFY                               0x32 /** The slave internal program verify routine detects an error */
#define XCP_ERR_RESOURCE_TEMPORARY_NOT_ACCESSIBLE    0x33
#define XCP_ERR_OK_PENDING                           0xFE /** The request was processed successfully but there are some pending tasks to be performed. (for internal use only) */
#define XCP_ERR_OK                                   0xFF

#define XCP_PID_CMD_CONNECT                     0xFF
#define XCP_PID_CMD_DISCONNECT                  0xFE
#define XCP_PID_CMD_GET_STATUS                  0xFD
#define XCP_PID_CMD_SYNCH                       0xFC
#define XCP_PID_CMD_GET_COMM_MODE_INFO          0xFB
#define XCP_PID_CMD_GET_ID                      0xFA
#define XCP_PID_CMD_SET_REQUEST                 0xF9
#define XCP_PID_CMD_GET_SEED                    0xF8
#define XCP_PID_CMD_UNLOCK                      0xF7
#define XCP_PID_CMD_SET_MTA                     0xF6
#define XCP_PID_CMD_UPLOAD                      0xF5
#define XCP_PID_CMD_SHORT_UPLOAD                0xF4
#define XCP_PID_CMD_BUILD_CHECKSUM              0xF3
#define XCP_PID_CMD_TRANSPORT_LAYER_CMD         0xF2
#define XCP_PID_CMD_USER_CMD                    0xF1
#define XCP_PID_CMD_DOWNLOAD                    0xF0
#define XCP_PID_CMD_DOWNLOAD_NEXT               0xEF
#define XCP_PID_CMD_DOWNLOAD_MAX                0xEE
#define XCP_PID_CMD_SHORT_DOWNLOAD              0xED
#define XCP_PID_CMD_MODIFY_BITS                 0xEC
#define XCP_PID_CMD_SET_CAL_PAGE                0xEB
#define XCP_PID_CMD_GET_CAL_PAGE                0xEA
#define XCP_PID_CMD_GET_PAG_PROCESSOR_INFO      0xE9
#define XCP_PID_CMD_GET_SEGMENT_INFO            0xE8
#define XCP_PID_CMD_GET_PAGE_INFO               0xE7
#define XCP_PID_CMD_SET_SEGMENT_INFO            0xE6
#define XCP_PID_CMD_GET_SEGMENT_MODE            0xE5
#define XCP_PID_CMD_COPY_CAL_PAGE               0xE4
#define XCP_PID_CMD_SET_DAQ_PTR                 0xE2
#define XCP_PID_CMD_WRITE_DAQ                   0xE1
#define XCP_PID_CMD_SET_DAQ_LIST_MODE           0xE0
#define XCP_PID_CMD_START_STOP_DAQ_LIST         0xDE
#define XCP_PID_CMD_START_STOP_SYNCH            0xDD
#define XCP_PID_CMD_WRITE_DAQ_MULTIPLE          0xC7
#define XCP_PID_CMD_READ_DAQ                    0xDB
#define XCP_PID_CMD_GET_DAQ_CLOCK               0xDC
#define XCP_PID_CMD_GET_DAQ_PROCESSOR_INFO      0xDA
#define XCP_PID_CMD_GET_DAQ_RESOLUTION_INFO     0xD9
#define XCP_PID_CMD_GET_DAQ_LIST_MODE           0xDF
#define XCP_PID_CMD_GET_DAQ_EVENT_INFO          0xD7
#define XCP_PID_CMD_CLEAR_DAQ_LIST              0xE3
#define XCP_PID_CMD_GET_DAQ_LIST_INFO           0xD8
#define XCP_PID_CMD_FREE_DAQ                    0xD6
#define XCP_PID_CMD_ALLOC_DAQ                   0xD5
#define XCP_PID_CMD_ALLOC_ODT                   0xD4
#define XCP_PID_CMD_ALLOC_ODT_ENTRY             0xD3
#define XCP_PID_CMD_PROGRAM_START               0xD2
#define XCP_PID_CMD_PROGRAM_CLEAR               0xD1
#define XCP_PID_CMD_PROGRAM                     0xD0
#define XCP_PID_CMD_PROGRAM_RESET               0xCF
#define XCP_PID_CMD_GET_PGM_PROCESSOR_INFO      0xCE
#define XCP_PID_CMD_GET_SECTOR_INFO             0xCD
#define XCP_PID_CMD_PROGRAM_PREPARE             0xCC
#define XCP_PID_CMD_PROGRAM_FORMAT              0xCB
#define XCP_PID_CMD_PROGRAM_NEXT                0xCA
#define XCP_PID_CMD_PROGRAM_MAX                 0xC9
#define XCP_PID_CMD_PROGRAM_VERIFY              0xC8

#define XCP_PID_RES        0xFF
#define XCP_PID_ERR        0xFE
#define XCP_PID_EV         0xFD
#define XCP_PID_SERV       0xFC
#define XCP_PID_DAQ_LOW    0x00
#define XCP_PID_DAQ_HIGH   0xFB
#define XCP_PID_STIM_LOW   0x00
#define XCP_PID_STIM_HIGH  0xBF



#endif /* INC_XCP_H_ */
