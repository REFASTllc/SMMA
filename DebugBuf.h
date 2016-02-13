#ifndef DEBUG_BUF_H
#define DEBUG_BUF_H


#define DEBUG_ON

#ifdef DEBUG_ON

#define LOG_BUF_DEPTH   256
    
#define LOGP(p)   DebugLog(p);

typedef enum
{
    LE_NOT_USED = 0,
    ENTER_IN_ISR,
    OUT_OF_ISR,
            
    TX_UART1,
    RX_UART1,
    ERR_UART1,
    TX_UART2,
    RX_UART2,
    ERR_UART2,
            
    TIMER1,
    TIMER2,
    TIMER3, 
    TIMER4,
    TIMER5,
    TIMER23,
    TIMER45,
            
    IC1,
    IC2,
    
    ADC,
            
    I2C1,
    I2C2,
            
    EXT_INT1,
    EXT_INT2,

    RX_SPI1,
    TX_SPI1,
    ERR_SPI1,

    DUMMY
} E_LOG_EVENT;

typedef struct
{
    unsigned char buffer[LOG_BUF_DEPTH];
    unsigned short actualPos;
} S_RING_BUF;
  
void InitDebugBuf(void);
void DebugLog(E_LOG_EVENT event);

#else
  #define LOGP(a)              {} 
#endif

  
#endif // ifndef DEBUG_BUF_H
