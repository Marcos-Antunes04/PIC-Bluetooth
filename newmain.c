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

uint8_t UART_RX_Buffer = 0x33;
char str[18] = "BOTAO PRESSIONADO\n";
uint8_t estado = 0;
uint8_t estado_botao = 0;

void UART_RX_Init(){
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
 
void UART_Init(){
    
    TXSTA = 0x20;         //BRGH=0, TXEN = 1, Asynchronous Mode, 8-bit mode
    RCSTA = 0b10010000;   //Serial Port enabled,8-bit reception
    SPBRG = 32;           //9600 baudrate for 20Mhz
    INTCON = 0xC0;        //Configura o registrador de interrupções
    PIE1 = 0x20;
}
    
void UART_Transmit_Char(uint8_t a){
    TXREG=a;
    while(!TXIF);
    TXIF = 0;
}

void UART_Transmit_String(uint8_t *a,int size){
    for(int i = 0; i < size; i++){
        UART_Transmit_Char(a[i]);
        __delay_ms(1);
    }
}

unsigned char UART_Receive(){
    while(!RCIF);
    RCIF=0;
    return RCREG;
}

void __interrupt() ISR(){
    if(RCIF == 1){
        UART_RX_Buffer = UART_Receive();
        if(UART_RX_Buffer == 'f' & (PORTBbits.RB1 == 1))
            PORTBbits.RB1 = 0;
        if(UART_RX_Buffer == 'n' & (PORTBbits.RB1 == 0))
            PORTBbits.RB1 = 1;
        estado = 1;
        RCIF = 0;
    }
    if (INTF == 1) {
        __delay_ms(80);     // Delay anti debouncing
        estado_botao = 1;
        INTF = 0;  // Limpa a flag de interrupção externa
    }
}

void main(void) {
  TRISC6=1;
  TRISC7=1;
  TRISB = 0X00;
  PORTB = 0x00;
  UART_Init();  // Configuração inicial do UART
  INTE = 1;     // Habilita interrupção do pino RB0
  while(1)
  {
    if(estado){
        UART_Transmit_String("Mensagem recebida\n",18);
        estado = 0;
    }
    if(estado_botao){
        UART_Transmit_String((uint8_t*)str, 18); // Transmite a string "BOTAO ACIONADO"
        estado_botao = 0;
    }
  }
}
