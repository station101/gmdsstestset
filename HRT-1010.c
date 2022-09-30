/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
© Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : MF/HF TESTER 
Version : 1.0 
Date    : 30.12.2021
Author  : Oleg Taran
Company : GMDSS Test Set
Comments: The program generates a test message and performs control of a DDS synthesizer to generate a modulated AM / FSK signal. HRT-1010 device version  has is capable to generate DSC and MORSE code messages at carrier frequency 2187,5 kHz or 4207,5 kHz


Chip type               : ATtiny2313A 4 MHz / AD9832 25 MHz
Memory model        : Tiny
External RAM size    : 0
Data Stack size       : 64
*******************************************************/
#include <tiny2313a.h>
#include <delay.h>

#define YY      0b1111111111
#define BB      0b0000000000
#define BY      0b0101010101
#define	N000	0b0000000111
#define	N001	0b1000000110
#define	N002	0b0100000110
#define	N003	0b1100000101
#define	N004	0b0010000110
#define	N005	0b1010000101
#define	N006	0b0110000101
#define	N007	0b1110000100
#define	N008	0b0001000110
#define	N009	0b1001000101
#define	N010	0b0101000101
#define	N011	0b1101000100
#define	N012	0b0011000101
#define	N013	0b1011000100
#define	N014	0b0111000100
#define	N015	0b1111000011
#define	N016	0b0000100110
#define	N017	0b1000100101
#define	N018	0b0100100101
#define	N019	0b1100100100
#define	N020	0b0010100101
#define	N021	0b1010100100
#define	N022	0b0110100100
#define	N023	0b1110100011
#define	N024	0b0001100101
#define	N025	0b1001100100
#define	N026	0b0101100100
#define	N027	0b1101100011
#define	N028	0b0011100100
#define	N029	0b1011100011
#define	N030	0b0111100011
#define	N031	0b1111100010
#define	N032	0b0000010110
#define	N033	0b1000010101
#define	N034	0b0100010101
#define	N035	0b1100010100
#define	N036	0b0010010101
#define	N037	0b1010010100
#define	N038	0b0110010100
#define	N039	0b1110010011
#define	N040	0b0001010101
#define	N041	0b1001010100
#define	N042	0b0101010100
#define	N043	0b1101010011
#define	N044	0b0011010100
#define	N045	0b1011010011
#define	N046	0b0111010011
#define	N047	0b1111010010
#define	N048	0b0000110101
#define	N049	0b1000110100
#define	N050	0b0100110100
#define	N051	0b1100110011
#define	N052	0b0010110100
#define	N053	0b1010110011
#define	N054	0b0110110011
#define	N055	0b1110110010
#define	N056	0b0001110100
#define	N057	0b1001110011
#define	N058	0b0101110011
#define	N059	0b1101110010
#define	N060	0b0011110011
#define	N061	0b1011110010
#define	N062	0b0111110010
#define	N063	0b1111110001
#define	N064	0b0000001110
#define	N065	0b1000001101
#define	N066	0b0100001101
#define	N067	0b1100001100
#define	N068	0b0010001101
#define	N069	0b1010001100
#define	N070	0b0110001100
#define	N071	0b1110001011
#define	N072	0b0001001101
#define	N073	0b1001001100
#define	N074	0b0101001100
#define	N075	0b1101001011
#define	N076	0b0011001100
#define	N077	0b1011001011
#define	N078	0b0111001011
#define	N079	0b1111001010
#define	N080	0b0000101101
#define	N081	0b1000101100
#define	N082	0b0100101100
#define	N083	0b1100101011
#define	N084	0b0010101100
#define	N085	0b1010101011
#define	N086	0b0110101011
#define	N087	0b1110101010
#define	N088	0b0001101100
#define	N089	0b1001101011
#define	N090	0b0101101011
#define	N091	0b1101101010
#define	N092	0b0011101011
#define	N093	0b1011101010
#define	N094	0b0111101010
#define	N095	0b1111101001
#define	N096	0b0000011101
#define	N097	0b1000011100
#define	N098	0b0100011100
#define	N099	0b1100011011
#define	N100	0b0010011100
#define	N101	0b1010011011
#define	N102	0b0110011011
#define	N103	0b1110011010
#define	N104	0b0001011100
#define	N105	0b1001011011
#define	N106	0b0101011011
#define	N107	0b1101011010
#define	N108	0b0011011011
#define	N109	0b1011011010
#define	N110	0b0111011010
#define	N111	0b1111011001
#define	N112	0b0000111100
#define	N113	0b1000111011
#define	N114	0b0100111011
#define	N115	0b1100111010
#define	N116	0b0010111011
#define	N117	0b1010111010
#define	N118	0b0110111010
#define	N119	0b1110111001
#define	N120	0b0001111011
#define	N121	0b1001111010
#define	N122	0b0101111010
#define	N123	0b1101111001
#define	N124	0b0011111010
#define	N125	0b1011111001
#define	N126	0b0111111001
#define	N127	0b1111111000

#define BUT_CHNL      PIND.2
#define BUT_MODE      PIND.0
#define BUT_DEV       PIND.3

#define LED_4207      PORTB.0 //верхний
#define LED_2187      PORTD.1 //средний
#define LED_TX        PORTB.1 //нижний

#define DCLK          PORTB.2
#define AUDIO         PORTB.3

#define AD9832_FSEL   PORTB.4
#define AD9832_FSYNC  PORTD.4
#define AD9832_SCLK   PORTD.5
#define AD9832_SDATA  PORTD.6

#define OCRH_1615HZ   0x04 
#define OCRL_1615HZ   0xD5
#define OCRH_1785HZ   0x04
#define OCRL_1785HZ   0x5F

//25000000
flash unsigned long int FREQ[]= 
{
0x16662D5C, //2187415  ok   
0x16669F71, //2187585  ok     
0x2B157C69, //4207415  ok    
0x2B15EE7F, //4207585  ok

0x166A11ED, //2187500+1400=2188900      
0x166A11ED, //2187500           
0x2B1960FA, //4207500+1400=4208900      
0x2B1960FA, //4207500          
};

//MORSE CODE R-REC-M.1677
flash unsigned int MORSE_MSG[]=
{
0b0000000000,
0b0000000000,
0b0000000000,
0b0000000000,
0b0000000000,
0b0000000000,
0b0000000000,
0b1111111111,
0b1111111111,
0b1111111111,
0b0000000000,
0b0000000000,
0b0000000000,
0b1111111111,
0b0000000000,
0b0000000000,
0b0000000000,
0b1111111111,
0b0000000000,
0b1111111111,
0b0000000000,
0b1111111111,
0b0000000000,
0b0000000000,
0b0000000000,
0b1111111111,
0b1111111111,
0b1111111111,
0b0000000000,
0b0000000000,
0b0000000000,
0b0000000000,
0b0000000000,
0b0000000000,
0b0000000000,
0b1111111111,
0b0000000000,
0b1111111111,
0b0000000000,
0b1111111111,
0b0000000000,
0b0000000000,
0b0000000000,
0b1111111111,
0b1111111111,
0b1111111111,
0b0000000000,
0b1111111111,
0b1111111111,
0b1111111111,
0b0000000000,
0b1111111111,
0b1111111111,
0b1111111111,
0b0000000000,
0b0000000000,
0b0000000000,
0b1111111111,
0b0000000000,
0b1111111111,
0b0000000000,
0b1111111111,
0b0000000000,
0b0000000000,
0b0000000000,
0b0000000000,
0b0000000000,
0b0000000000,
0b0000000000
};

//DSC CODE ћ—Ё-R M.493-15                                                                                                       
flash unsigned int DSC_MSG[]=
{
//DX   RX POSITIONS
BB,   BB,
BB,   BB,
BB,   BB,
BB,   BB,
BB,   BB,
BB,   BB,
BB,   BB,
BB,   BB,
BB,   BB,
BB,   BB,
YY,   YY,
YY,   YY,
YY,   YY,
YY,   YY,
YY,   YY,
YY,   YY,
YY,   YY,
YY,   YY,
YY,   YY,
YY,   YY,
BY,   BY,    
BY,   BY,    
BY,   BY,    
BY,   BY,    
BY,   BY,    
N125, N111,  //DX
N125, N110,  //DX
N125, N109,  //DX
N125, N108,  //DX
N125, N107,  //DX
N125, N106,  //DX
N112, N105,  //A
N112, N104,  //A     
N027, N112,  //MMSI  
N021, N112,  //MMSI  
N070, N027,  //MMSI
N000, N021,  //MMSI
N000, N070,  //MMSI
N102, N000,  //TYPE
N099, N000,  //POS1
N099, N102,  //POS1
N099, N099,  //POS1
N099, N099,  //POS1
N099, N099,  //POS1
N088, N099,  //UTC 
N088, N099,  //UTC 
N113, N088,  //FEC 
N127, N088,  //EOS 
N051, N113,  //ECC
N127, N127,  //EOS
N127, N051,  //EOS
N000
};

unsigned char fsel,next_bit,ocrl,ocrh,mode,chnl; 

//Init AVR
void AVR_ini(unsigned char i)
{
// Declare your local variables here

// Crystal Oscillator division factor: 1
#pragma optsize-
CLKPR=(1<<CLKPCE);
CLKPR=(0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif

// Input/Output Ports initialization
// Port A initialization
// Function: Bit2=In Bit1=In Bit0=In 
DDRA=(0<<DDA2) | (0<<DDA1) | (0<<DDA0);
// State: Bit2=T Bit1=T Bit0=T 
PORTA=(0<<PORTA2) | (0<<PORTA1) | (0<<PORTA0);

// Port B initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=Out Bit3=Out Bit2=Out Bit1=Out Bit0=Out 
DDRB=(0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (1<<DDB4) | (1<<DDB3) | (1<<DDB2) | (1<<DDB1) | (1<<DDB0);
// State: Bit7=T Bit6=T Bit5=T Bit4=0 Bit3=0 Bit2=0 Bit1=0 Bit0=0 
PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);

// Port D initialization
// Function: Bit6=Out Bit5=Out Bit4=Out Bit3=In Bit2=In Bit1=Out Bit0=In 
DDRD=(1<<DDD6) | (1<<DDD5) | (1<<DDD4) | (0<<DDD3) | (0<<DDD2) | (1<<DDD1) | (0<<DDD0);
// State: Bit6=0 Bit5=0 Bit4=1 Bit3=P Bit2=P Bit1=0 Bit0=P 
PORTD=(0<<PORTD6) | (0<<PORTD5) | (1<<PORTD4) | (1<<PORTD3) | (1<<PORTD2) | (0<<PORTD1) | (1<<PORTD0);

if(i)
    {
    // Timer/Counter 0 initialization
    // Clock source: System Clock
    // Clock value: 62,500 kHz
    // Mode: CTC top=OCR0A
    // OC0A output: Disconnected
    // OC0B output: Disconnected
    // Timer Period: 0,992 ms
    TCCR0A=(0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (1<<WGM01) | (0<<WGM00);
    TCCR0B=(0<<WGM02) | (0<<CS02) | (1<<CS01) | (1<<CS00);
    TCNT0=0x00;
    OCR0A=0x3D;
    OCR0B=0x00;
    }
else
    {
    // Timer/Counter 0 initialization
    // Clock source: System Clock
    // Clock value: 15,625 kHz
    // Mode: CTC top=OCR0A
    // OC0A output: Disconnected
    // OC0B output: Disconnected
    // Timer Period: 9,984 ms
    TCCR0A=(0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (1<<WGM01) | (0<<WGM00);
    TCCR0B=(0<<WGM02) | (1<<CS02) | (0<<CS01) | (0<<CS00);
    TCNT0=0x00;
    OCR0A=0x9B;
    OCR0B=0x00;


    // Timer/Counter 1 initialization
    // Clock source: System Clock
    // Clock value: 4000,000 kHz
    // Mode: CTC top=OCR1A
    // OC1A output: Toggle on compare match
    // OC1B output: Disconnected
    // Noise Canceler: Off
    // Input Capture on Falling Edge
    // Timer Period: 0,25 us
    // Output Pulse(s):
    // OC1A Period: 0,5 us Width: 0,25 us
    // Timer1 Overflow Interrupt: Off
    // Input Capture Interrupt: Off
    // Compare A Match Interrupt: Off
    // Compare B Match Interrupt: Off
    TCCR1A=(0<<COM1A1) | (1<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
    TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10);
    TCNT1H=0x00;
    TCNT1L=0x00;
    ICR1H=0x00;
    ICR1L=0x00;
    OCR1AH=0x00;
    OCR1AL=0x00;
    OCR1BH=0x00;
    OCR1BL=0x00;
    }; 

    // Timer(s)/Counter(s) Interrupt(s) initialization
    TIMSK=(0<<TOIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<ICIE1) | (0<<OCIE0B) | (0<<TOIE0) | (1<<OCIE0A);

// Global enable interrupts
//#asm("sei")
}

//Soft SPI
static void spi_soft(unsigned char byte)
{
static unsigned char  i;
for (i=8; i; i--)
    {
    if (byte&0x80)	AD9832_SDATA=1;  //бит данных
    else            AD9832_SDATA=0;
    AD9832_SCLK=0;	               	//строб импульс
    #asm("nop")
    byte<<=1;
    AD9832_SCLK=1;
    }
}

//Write to data register
void AD9832_write(unsigned  char COMMAND, unsigned char DATA)
{
//C3 C2 C1 C0 A3 A2 A1 A0 D7 D6 D5 D4 D3 D2 D1 D0
AD9832_FSYNC=0;
spi_soft(COMMAND); 		        //14 LSBs
spi_soft(DATA); 				//14 MSBs
AD9832_FSYNC=1;
}

//Reset
void AD9832_reset(void)
{
AD9832_write(0b11111000,0b00000000);             //Reset DDS
}

//Sleep
void AD9832_sleep(unsigned char sleep)
{
if (sleep) AD9832_write(0b11100000,0b00000000);   //sleep
else AD9832_write(0b11000000,0b00000000);  //work
}

//Load Frequency Registers
void AD9832_loadf(unsigned long int f0, unsigned long int f1)
{
AD9832_write(0b00110011,(unsigned char)(f0>>24));   //Set inital FREQ0 REG 8 H MSBs
AD9832_write(0b00100010,(unsigned char)(f0>>16));   //Set inital FREQ0 REG 8 L MSBs
AD9832_write(0b00110001,(unsigned char)(f0>>8));   //Set inital FREQ0 REG 8 H LSBs
AD9832_write(0b00100000,(unsigned char)f0);   //Set inital FREQ0 REG 8 L LSBs
AD9832_write(0b00110111,(unsigned char)(f1>>24));   //Set inital FREQ1 REG 8 H MSBs
AD9832_write(0b00100110,(unsigned char)(f1>>16));   //Set inital FREQ1 REG 8 L MSBs 
AD9832_write(0b00110101,(unsigned char)(f1>>8));   //Set inital FREQ1 REG 8 H LSBs
AD9832_write(0b00100100,(unsigned char)f1);   //Set inital FREQ1 REG 8 L LSBs
}

//DDS ini
void AD9832_ini(unsigned char c, unsigned char m) 
{    
#asm("cli")
AD9832_reset();
AD9832_loadf(FREQ[0+2*c+4*m],FREQ[1+2*c+4*m]);
AD9832_sleep(0);
#asm("sei")
}

//Mode selection
unsigned char setting(void)
{
static unsigned char c,m;
if (BUT_CHNL==0)
    {                                                                  
    delay_ms(20); while (BUT_CHNL==0) {};
    if (c) c=0; else c=1;                                             
    }; 
m=BUT_MODE;
switch(c)
    {
    case 0: LED_2187=1; LED_4207=0;  break;
    case 1: LED_2187=0; LED_4207=1;  break;
    };
if ((chnl!=c)||(mode!=m)) 
    {
    chnl=c;
    mode=m;
    return 1;
    };
return 0;
}

//Send symbol
void send_symbol(unsigned int symbol)
{
static unsigned char i;
for(i=10; i>0; i--) 
    {
    #asm("cli")
    if (symbol&(1<<(i-1)))  {fsel=0; DDRB.3=1; ocrh=OCRH_1615HZ; ocrl=OCRL_1615HZ;} 
    else                    {fsel=1; if (mode) DDRB.3=0; else {DDRB.3=1; ocrh=OCRH_1785HZ; ocrl=OCRH_1785HZ;};}; 
    next_bit=1;
    #asm("sei")
    while(next_bit){LED_TX=fsel;};
    }
}

//Generate a message
void send_msg(unsigned int flash *msg, unsigned char size)
{
static unsigned char i=0;
for(i=0; i<size; i++) 
    {
    send_symbol(msg[i]);
    if (setting()) {AD9832_ini(chnl,mode); return;};
    };    
}

//Timer0 output compare A interrupt service routine   100 Hz
interrupt [TIM0_COMPA] void timer0_compa_isr(void)
{
if (mode) AD9832_sleep(fsel); else AD9832_FSEL=fsel;
next_bit=0;
TIMSK|=(1<<OCIE1A); //Enable interrupt 
TIFR|=(1<<OCF1A);   //Reset interrupt flag
}

//Timer1 output compare A interrupt service routine  1700 +-85 Hz
interrupt [TIM1_COMPA] void timer1_compa_isr(void)
{
TIMSK&=~(1<<OCIE1A); //Disable interrupt until next bit is transmitted
OCR1AH=ocrh; OCR1AL=ocrl;
}

//Program cycle
void   main(void)
{
AVR_ini(0);
setting();
AD9832_ini(chnl,mode);
while (1)
    {      
    if (mode) send_msg(MORSE_MSG,67); else send_msg(DSC_MSG,103);   
    } 
}

