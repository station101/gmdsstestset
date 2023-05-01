/*******************************************************
Project			: NMT-1010 v1.0 23.02.2023
Author			: Oleg Taran 
Chip type               : ATmega88PA
Program type            : Application
AVR Core Clock frequency: 7,372800 MHz
*******************************************************/

#include <mega88a.h>
#include <stdio.h>
#include <delay.h>
#include <stdlib.h> 

#define RX          PIND.0
#define TX          PORTD.1
#define TXP         PORTB.2

#define LEDR        PORTD.2
#define LEDG        PORTD.4
#define LEDB        PORTD.3

#define LED5        PORTD.5
#define LED4        PORTD.6
#define LED3        PORTD.7
#define LED2        PORTB.0
#define LED1        PORTB.1

#define DEC_BUT        PINC.0
#define SET_BUT        PINC.1
#define INC_BUT        PINC.2

#define DEC     1
#define SET     2
#define INC     3

#define GLL     0
#define HDT     1
#define DPT     2
#define VBW     3
#define RSA     4
#define BR      5


signed int data[6];
unsigned char key,mode,msg,count,h,m,s,ms;

//$GPGLL,4542.5020,N,02817.1540,E,192141.99,A*06
unsigned char GLL_str[]="GPGLL,4542.5020,N,02817.1540,E,192141.99,A";     

//$HEHDT  $--HDT, x.x, T*hh<CR><LF>
unsigned char HDT_str[]="HCHDT,354.0,T";       

//$SDDBT, x.x, f, x.x, M, x.x, F*hh<CR><LF>
unsigned char DBT_str[]="SDDBT,,,10.0,M,,"; 

//$VMVBW, x.x, x.x, A, x.x, x.x, A, x.x, A, x.x, A*hh<CR><LF>
unsigned char VBW_str[]="VMVBW,,,,,10.0,N,,"; 

//$RCRSA,0.0,A*38    $--RSA,x.x,A,,
unsigned char RSA_str[]="RCRSA,00.0,A,,";   

//$GPRSA,-0.1,A*38
unsigned char mRSA_str[]="RCRSA,-00.1,A,,";  


void cpu_ini(void)
{
// Crystal Oscillator division factor: 1
#pragma optsize-
CLKPR=(1<<CLKPCE);
CLKPR=(0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif

// Input/Output Ports initialization
// Port B initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=Out Bit1=Out Bit0=Out 
DDRB=(0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (0<<DDB3) | (1<<DDB2) | (1<<DDB1) | (1<<DDB0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=0 Bit1=0 Bit0=0 
PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);

// Port C initialization
// Function: Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRC=(0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (0<<DDC1) | (0<<DDC0);
// State: Bit6=T Bit5=T Bit4=T Bit3=T Bit2=P Bit1=P Bit0=P 
PORTC=(0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (1<<PORTC2) | (1<<PORTC1) | (1<<PORTC0);

// Port D initialization
// Function: Bit7=Out Bit6=Out Bit5=Out Bit4=Out Bit3=Out Bit2=Out Bit1=Out Bit0=In 
DDRD=(1<<DDD7) | (1<<DDD6) | (1<<DDD5) | (1<<DDD4) | (1<<DDD3) | (1<<DDD2) | (1<<DDD1) | (0<<DDD0);
// State: Bit7=0 Bit6=0 Bit5=0 Bit4=0 Bit3=0 Bit2=0 Bit1=0 Bit0=T 
PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: 7,200 kHz
// Mode: Normal top=0xFF
// OC0A output: Disconnected
// OC0B output: Disconnected
// Timer Period: 10 ms
TCCR0A=(0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (0<<WGM01) | (0<<WGM00);
TCCR0B=(0<<WGM02) | (1<<CS02) | (0<<CS01) | (1<<CS00);
TCNT0=0xB8;
OCR0A=0x00;
OCR0B=0x00;

// Timer/Counter 0 Interrupt(s) initialization
TIMSK0=(0<<OCIE0B) | (0<<OCIE0A) | (1<<TOIE0);

// Global enable interrupts
#asm("sei")
}

void uart_ini(unsigned char b)
{
// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: On
// USART Transmitter: On
// USART0 Mode: Asynchronous
UCSR0A=(0<<RXC0) | (0<<TXC0) | (0<<UDRE0) | (0<<FE0) | (0<<DOR0) | (0<<UPE0) | (0<<U2X0) | (0<<MPCM0);
UCSR0B=(0<<RXCIE0) | (0<<TXCIE0) | (0<<UDRIE0) | (1<<RXEN0) | (1<<TXEN0) | (0<<UCSZ02) | (0<<RXB80) | (0<<TXB80);
UCSR0C=(0<<UMSEL01) | (0<<UMSEL00) | (0<<UPM01) | (0<<UPM00) | (0<<USBS0) | (1<<UCSZ01) | (1<<UCSZ00) | (0<<UCPOL0);

switch(b)
    {
    case 0: {UBRR0H=0x00; UBRR0L=0xBF; break;};  // USART Baud Rate: 2400
    case 1: {UBRR0H=0x00; UBRR0L=0x5F; break;};  // USART Baud Rate: 4800
    case 2: {UBRR0H=0x00; UBRR0L=0x2F; break;};  // USART Baud Rate: 9600
    case 3: {UBRR0H=0x00; UBRR0L=0x0B; break;};  // USART Baud Rate: 38400
    case 4: {UBRR0H=0x00; UBRR0L=0x07; break;};  // USART Baud Rate: 57600
    };     
}

unsigned int checksum(unsigned char *s) 
{
unsigned int c=0;
while (*s) {c^=*s++;};
return c;
}

void scan_but(void)     //scan key
{
static unsigned char hold;

if (INC_BUT==0) {delay_ms(100); if (INC_BUT==0){hold++; key=INC;};} else hold=0;
if (DEC_BUT==0) {delay_ms(100); if (DEC_BUT==0){hold++; key=DEC;};} else hold=0;
if (SET_BUT==0) {delay_ms(100); if (SET_BUT==0){hold++; key=SET;};} else hold=0;   
if (key==SET) {key=0; if (mode<3) mode++; else mode=0;};

switch(mode)
    {
    case 0:         //send all msg
        {
        break; 
        };     
    case 1:         //select msg
        {
        if (key==INC) {key=0; if (msg<4) msg++; else msg=4;};  
        if (key==DEC) {key=0; if (msg>0) msg--; else msg=0;};
        break;   
        };   
    case 2:         //select data
        {
        switch(msg)
            {
            case 0:  //position
                {
                if (key==INC) {key=0; if (data[GLL]<1000) data[GLL]++; else data[GLL]=1000;};  
                if (key==DEC) {key=0; if (data[GLL]>0) data[GLL]--; else data[GLL]=0;};
                break;   
                };         
            case 1:  //heading
                {
                if (key==INC) {key=0; if (data[HDT]<3600) data[HDT]++; else data[HDT]=3600;};  
                if (key==DEC) {key=0; if (data[HDT]>0) data[HDT]--; else data[HDT]=0;};
                break;                          
                };   
            case 2:   //deps
                {
                if (key==INC) {key=0; if (data[DPT]<1000) data[DPT]++; else data[DPT]=1000;};  
                if (key==DEC) {key=0; if (data[DPT]>0) data[DPT]--; else data[DPT]=0;}; 
                break;                         
                };   
            case 3:   //velocity
                {
                if (key==INC) {key=0; if (data[VBW]<100) data[VBW]++; else data[VBW]=100;};  
                if (key==DEC) {key=0; if (data[VBW]>0) data[VBW]--; else data[VBW]=0;};
                break;                          
                };   
            case 4:   //ruderangle
                {
                if (key==INC) {key=0; if (data[RSA]<400) data[RSA]++; else data[RSA]=400;};  
                if (key==DEC) {key=0; if (data[RSA]>-400) data[RSA]--; else data[RSA]=-400;};
                break;                          
                };   
            };
        break;  
        };   
    case 3:         //select bitrate
        {
        if (key==INC) {key=0; if (data[BR]<4) data[BR]++; else data[BR]=4;};  
        if (key==DEC) {key=0; if (data[BR]>0) data[BR]--; else data[BR]=0;};
        break;   
        };   
    };
}

void set_led(unsigned char mode,unsigned char msg,unsigned char brt)
{
static unsigned char i,j;
switch(mode)
    {
    case 0:      //send all msg
        {
        LEDR=0; LEDG=1; LEDB=0;
        switch(msg)
            {
            case 0: {LED1=0; LED2=1; LED3=1; LED4=1; LED5=1; break;};
            case 1: {LED1=1; LED2=0; LED3=1; LED4=1; LED5=1; break;};
            case 2: {LED1=1; LED2=1; LED3=0; LED4=1; LED5=1; break;};
            case 3: {LED1=1; LED2=1; LED3=1; LED4=0; LED5=1; break;};
            case 4: {LED1=1; LED2=1; LED3=1; LED4=1; LED5=0; break;};
            };
        break;                
        };  
    case 1:      //select msg
        {
        LEDR=0; LEDG=1; LEDB=0;
        switch(msg)
            {
            case 0: {LED1=0; LED2=1; LED3=1; LED4=1; LED5=1; break;};
            case 1: {LED1=1; LED2=0; LED3=1; LED4=1; LED5=1; break;};
            case 2: {LED1=1; LED2=1; LED3=0; LED4=1; LED5=1; break;};
            case 3: {LED1=1; LED2=1; LED3=1; LED4=0; LED5=1; break;};
            case 4: {LED1=1; LED2=1; LED3=1; LED4=1; LED5=0; break;};
            };
        break;                 
        };   
    case 2:      //select data    мигает
        {
        i++;
        if (i>=2) {i=0; j=!j;};
        if (j) {LEDR=0; LEDG=0; LEDB=0;} else {LEDR=0; LEDG=1; LEDB=0;}
        switch(msg)
            {
            case 0: {LED1=0; LED2=1; LED3=1; LED4=1; LED5=1; break;};
            case 1: {LED1=1; LED2=0; LED3=1; LED4=1; LED5=1; break;};
            case 2: {LED1=1; LED2=1; LED3=0; LED4=1; LED5=1; break;};
            case 3: {LED1=1; LED2=1; LED3=1; LED4=0; LED5=1; break;};
            case 4: {LED1=1; LED2=1; LED3=1; LED4=1; LED5=0; break;};
            }; 
        break;                 
        };   
    case 3:      //select bitrate
        {
        LEDR=0; LEDG=0; LEDB=1;
        switch(brt)
            {
            case 0: {LED1=0; LED2=1; LED3=1; LED4=1; LED5=1; break;};
            case 1: {LED1=0; LED2=0; LED3=1; LED4=1; LED5=1; break;};
            case 2: {LED1=0; LED2=0; LED3=0; LED4=1; LED5=1; break;};
            case 3: {LED1=0; LED2=0; LED3=0; LED4=0; LED5=1; break;};
            case 4: {LED1=0; LED2=0; LED3=0; LED4=0; LED5=0; break;};
            };
        break;                
        };   
    };
}

void send_msg (unsigned char mode, signed int data, unsigned char h, unsigned char m, unsigned char s, unsigned char ms)
{
static signed char d0,d1,d2,d3,z,h1,h0,m1,m0,s1,s0,ms1,ms0;
static unsigned int d;

//ruder
if (data<0) z=1; else z=0;
d=abs(data); 

//variable
d3=d/1000;
d=d%1000;           
d2=d/100;
d=d%100;
d1=d/10;
d0=d%10;

//time
h1=h/10;
h0=h%10;
m1=m/10;
m0=m%10;
s1=s/10;
s0=s%10;
ms1=ms/10;
ms0=ms%10;

switch(mode)
    {
    case 0: 
        {
        GLL_str[24]=0x30+d3;
        GLL_str[25]=0x30+d2;
        GLL_str[26]=0x30+d1;
        GLL_str[27]=0x30+d0;
        GLL_str[31]=0x30+h1;
        GLL_str[32]=0x30+h0;
        GLL_str[33]=0x30+m1;
        GLL_str[34]=0x30+m0;
        GLL_str[35]=0x30+s1;
        GLL_str[36]=0x30+s0;
        GLL_str[38]=0x30+ms1;
        GLL_str[39]=0x30+ms0;
        putchar('$'); printf("%s*%02X\r\n", GLL_str, checksum(GLL_str)); 
        break;
        };
    case 1: 
        {
        HDT_str[6]=0x30+d3;
        HDT_str[7]=0x30+d2;
        HDT_str[8]=0x30+d1;
        HDT_str[10]=0x30+d0;   
        putchar('$'); printf("%s*%02X\r\n", HDT_str, checksum(HDT_str)); 
        break;};
    case 2: 
        {
        DBT_str[8]=0x30+d2;
        DBT_str[9]=0x30+d1;
        DBT_str[11]=0x30+d0;           
        putchar('$'); printf("%s*%02X\r\n", DBT_str, checksum(DBT_str));
        break;
        };
    case 3: 
        {
        VBW_str[10]=0x30+d2;
        VBW_str[11]=0x30+d1;
        VBW_str[13]=0x30+d0;        
        putchar('$'); printf("%s*%02X\r\n", VBW_str, checksum(VBW_str)); 
        break;
        };
    case 4: 
        {
        if (z) 
            {
            mRSA_str[7]=0x30+d2;
            mRSA_str[8]=0x30+d1;
            mRSA_str[10]=0x30+d0;       
            putchar('$'); printf("%s*%02X\r\n", mRSA_str, checksum(mRSA_str));
            }
        else
            {
            RSA_str[6]=0x30+d2;
            RSA_str[7]=0x30+d1;
            RSA_str[9]=0x30+d0;       
            putchar('$'); printf("%s*%02X\r\n", RSA_str, checksum(RSA_str));
            };
         
        break;
        };
    };
}
 

interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
TCNT0=0xB8;         // Reinitialize Timer 0 value
count++;
//часы
ms++;
if (ms==100) 
    {
    ms=0;
    s++;
    if (s==60) 
        {
        s=0;
        m++;
        if (m==60)
            {
            m=0;
            h++;
            if (h==24)
             {
             h=0;
             }
            };
        };
    };
}

void main(void)
{
static unsigned char br,i;

data[BR]=1;
cpu_ini();
while (1)
    {   
    if (br!=data[BR]) {br=data[BR]; uart_ini(br);} 
    if (count>=5) 
        {
        count=0;
        scan_but();  
        set_led(mode,msg,br); 
        i++;
        if (i==4)
            {
            i=0; 
            send_msg(msg,data[msg],h,m,s,ms); 
            if (mode==0) {if (msg<4) msg++; else msg=0;}; 
            };
        };
    };
}
