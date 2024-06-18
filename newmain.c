#include <xc.h>
#include <stdint.h>

#pragma config FOSC = HS
#pragma config WDTE = OFF 
#pragma config PWRTE = OFF 
#pragma config BOREN = ON
#pragma config LVP = OFF 
#pragma config CPD = OFF 
#pragma config WRT = OFF 
#pragma config CP = OFF 
#define _XTAL_FREQ 20000000


#define Blue_LED_ON    49
#define Blue_LED_OFF   50
#define Yellow_Toggle  51


void UART_RX_Init()
{
  BRGH = 1; // Set For High-Speed Baud Rate
  SPBRG = 25; // Set The Baud Rate To Be 9600 bps
  // Enable The Ascynchronous Serial Port
  SYNC = 0;
  SPEN = 1;
  // Set The RX-TX Pins to be in UART mode (not io)
  TRISC6 = 1; // As stated in the datasheet
  TRISC7 = 1; // As stated in the datasheet
  //--[ Enable UART Receiving Interrupts ]--
  RCIE = 1; // UART Receving Interrupt Enable Bit
  PEIE = 1; // Peripherals Interrupt Enable Bit
  GIE = 1; // Global Interrupt Enable Bit
  //------------------
  CREN = 1; // Enable Data Continous Reception
}
 
void UART_Init()
{
    TXSTA = 0x20; //BRGH=0, TXEN = 1, Asynchronous Mode, 8-bit mode
    RCSTA = 0b10010000; //Serial Port enabled,8-bit reception
    SPBRG = 32;           //9600 baudrate for 11.0592Mhz
    TXIF = 0;
    RCIF = 0;
    RCIE = 1;  // UART Receving Interrupt Enable Bit
    PEIE = 1;  // Peripherals Interrupt Enable Bit  
    GIE = 1;   // Global Interrupt Enable Bit
}
    
void UART_Transmit_Char(unsigned char a)
{
    TXREG=a;
    while(!TXIF);
    TXIF = 0;
}
void UART_Transmit_String(unsigned char *a,int size)
{
    for(int i = 0; i < size; i++){
        UART_Transmit_Char(a[i]);
        __delay_ms(1);
    }
}

unsigned char UART_Receive()
{
    while(!RCIF);
    RCIF=0;
    return RCREG;
}

uint8_t UART_RX_Buffer = 0;
void __interrupt() ISR(){
    if(PIR1bits.RCIF == 1){
        UART_RX_Buffer = RCREG;
        PIR1bits.RCIF;
    }
}

char *str = "Hello World";

void main(void) {
  TRISC6=1;
  TRISC7=1;
  TRISD=0;              //Port D is act as Output
  unsigned char get;
  UART_Init();  //---------------------------
  while(1)
  {
      UART_Transmit_String(str,10);
    __delay_ms(3000);
  }

}
