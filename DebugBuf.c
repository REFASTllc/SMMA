#include "DebugBuf.h"

#ifdef DEBUG_ON
  

typedef struct
{
    E_LOG_EVENT event;
    short delta;
} S_LOG;



S_RING_BUF debugBuf;

void InitDebugBuf(void)
{
    debugBuf.actualPos = 0;
}
 


void DebugLog(E_LOG_EVENT event)
{
    debugBuf.buffer[debugBuf.actualPos++] = event;
    if(debugBuf.actualPos == LOG_BUF_DEPTH)
        debugBuf.actualPos = 0;
}
  
#endif
