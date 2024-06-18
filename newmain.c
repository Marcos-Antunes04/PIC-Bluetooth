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


uint8_t UART_Buffer = 0;

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
 
void ser_int()
{
    TXSTA=0x20; //BRGH=0, TXEN = 1, Asynchronous Mode, 8-bit mode
    RCSTA=0b10010000; //Serial Port enabled,8-bit reception
    SPBRG=32;           //9600 baudrate for 11.0592Mhz
    TXIF=RCIF=0;
}
    
void tx(unsigned char a)
{
    TXREG=a;
    while(!TXIF);
    TXIF = 0;
}
void tx_string(unsigned char *a,int size)
{
    for(int i = 0; i < size; i++){
        tx(a[i]);
        __delay_ms(1);
    }
}

unsigned char rx()
{
    while(!RCIF);
    RCIF=0;
    return RCREG;
}

char *str = "Ola Mundo";

void main(void) {
  TRISC6=1;
  TRISC7=1;
  TRISD=0;              //Port D is act as Output
  unsigned char get;
  ser_int();  //---------------------------
  while(1)
  {
      tx_string(str,10);
    __delay_ms(3000);
  }

}
