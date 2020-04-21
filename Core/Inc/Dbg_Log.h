/*
 * Dbg_Log.h
 *
 *  Created on: Apr 20, 2020
 *      Author: maticpi
 */

#ifndef INC_DBG_LOG_H_
#define INC_DBG_LOG_H_

#include <inttypes.h>

#define DEBUG_LOG_LENGTH  1500

typedef struct DBG_STRUCT_T
{
  uint8_t dihanje_state;
  uint8_t ctr_mode;
  float target_value;     //either pressure or flow
  float measured_value;   //either pressure or flow
  float motor_speed;
  float motor_position;
  float motor_current;
} DbgItem_t;

void DLog(DbgItem_t* ptr_dbgItem);
void DLogReset();

#endif /* INC_DBG_LOG_H_ */
