/*
 * Dbg_log.c
 *
 *  Created on: Apr 20, 2020
 *      Author: maticpi
 */

#include "Dbg_Log.h"
#include <string.h>

DbgItem_t DebugLogBuffer[DEBUG_LOG_LENGTH];

int i_dbgLogIndex;
int i_dbgLogReadIndex;

void DLog(DbgItem_t* ptr_dbgItem)
{
  if (i_dbgLogIndex >= DEBUG_LOG_LENGTH) return;  //if log full, abort

  memcpy(&DebugLogBuffer[i_dbgLogIndex],ptr_dbgItem,sizeof(DbgItem_t));
  i_dbgLogIndex++;
}

void DLogReset()
{
  i_dbgLogIndex=0;
}

void TransmitDbgLog()
{

}
