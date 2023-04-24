/*******************************************************
Project : NVT-1010
Version : 1.1
Date    : 16.06.2021
Author  : Oleg Taran

Chip type               : ATtiny2313A 4.00 MHz
				: AD9832 12.80000 MHz

*******************************************************/
#include <tiny2313a.h>
#include <delay.h>

#define A       0x47
#define B       0x72
#define C       0x1D
#define D       0x53
#define E       0x56
#define F	    0x1B
#define	G	    0x35
#define	H	    0x69
#define	I	    0x4D
#define	J	    0x17
#define	K	    0x1E
#define	L	    0x65
#define	M	    0x39
#define	N	    0x59
#define	O	    0x71
#define	P	    0x2D
#define	Q	    0x2E
#define	R	    0x55
#define	S	    0x4B
#define	T	    0x74
#define	U	    0x4E
#define	V	    0x3C
#define	W	    0x27
#define	X	    0x3A
#define	Y	    0x2B
#define	Z	    0x63

#define	N0      0x2D
#define	N1      0x2E
#define	N2      0x27
#define	N3      0x56
#define	N4      0x55
#define	N5      0x74
#define	N6      0x2B
#define	N7      0x4E
#define	N8      0x4D
#define	N9      0x71

#define DOT     0x39
#define MINUS   0x47
#define PLUS    0x63
#define DDOT    0x1D

#define SPACE   0x5C    
#define BELL    0x17   
#define CR      0x78   
#define LF      0x6C   
#define LTRS    0x5A    
#define FIGS    0x36        
#define ALFA    0x0F  
define PHAS    0x66  

#define BUT_LH              PIND.0
#define BUT_FREQ            PIND.2
#define BUT_DEV             PIND.3

#define LED_4209            PORTB.0
#define LED_518             PORTD.1
#define LED_490             PORTB.1

#define DCLK_100            PORTB.2
#define AUDIO_1700          PORTB.3

#define AD9832_FSEL         PORTB.4
#define AD9832_FSYNC        PORTD.4
#define AD9832_SCLK         PORTD.5
#define AD9832_SDATA        PORTD.6

#define SYNC_CYCLE          40
#define TEST_CYCLE          40


flash unsigned char AD9832_FREQ[]= 
{
0x09,0xCC,0x56,0xD6,
0x09,0xCD,0x35,0xA8,
0x09,0xCC,0x5D,0x64,
0x09,0xCD,0x3C,0x36,
0x09,0xCC,0x63,0xF1,
0x09,0xCD,0x42,0xC4,
0x0A,0x5B,0xB2,0xFF,
0x0A,0x5C,0x91,0xD1,
0x0A,0x5B,0xB9,0x8C,
0x0A,0x5C,0x98,0x5F,
0x0A,0x5B,0xC0,0x1A,
0x0A,0x5C,0x9E,0xED,
0x54,0x30,0x2D,0xE0,
0x54,0x31,0x0C,0xB3,
0x54,0x30,0x34,0x6E,
0x54,0x31,0x13,0x40,
0x54,0x30,0x3A,0xFB,
0x54,0x31,0x19,0xCE,
};

flash unsigned char NUM[]=
{
0x2D,
0x2E,
0x27,
0x56,
0x55,
0x74,
0x2B,
0x4E,
0x4D,
0x71
};

flash unsigned char MSG0[]=                                          //79
{
Z,C,Z,C,SPACE,A,Z,FIGS,N0,N0,LTRS,CR,LF,                                                                //13
//D,A,N,M,A,R,FIGS,DOT,LTRS,C,O,M,FIGS,DOT,LTRS,U,A,SPACE,N,A,V,T,E,X,SPACE,T,E,S,T,E,R,CR,LF,            //33
D,A,N,U,B,E,FIGS,MINUS,LTRS,M,A,R,I,N,E,FIGS,DOT,LTRS,C,O,M,SPACE,N,A,V,T,E,X,T,CR,LF,            //31
C,A,L,I,B,R,A,T,E,D,SPACE,FIGS,N0,N3,DOT,N2,N0,N2,N3,LTRS,SPACE,                                        //21
F,W,FIGS,DDOT,SPACE,N1,DOT,N0,N0,LTRS,CR,LF                                                             //12
};                                                                                                          

flash unsigned char MSG11[]=                                         //27
{
C,E,N,T,R,E,SPACE,F,R,E,Q,U,E,N,C,Y,SPACE,T,E,S,T,SPACE,SPACE,SPACE,SPACE,CR,LF             
};

flash unsigned char MSG12[]=                                         //27
{
U,P,P,E,R,SPACE,F,R,E,Q,SPACE,T,O,L,E,R,A,N,C,E,SPACE,T,E,S,T,CR,LF 
};

flash unsigned char MSG13[]=                                         //27
{
L,O,W,E,R,SPACE,F,R,E,Q,SPACE,T,O,L,E,R,A,N,C,E,SPACE,T,E,S,T,CR,LF 
};

flash unsigned char MSG2[]=                                          //65
{
A,B,C,D,I,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,CR,LF,                                                      //28
FIGS,N0,N1,N2,N3,N4,N5,N6,N7,N8,N9,LTRS,SPACE,E,N,D,SPACE,T,E,S,T,FIGS,N0,N1,LTRS,CR,LF,                        //27
N,N,N,N,CR,LF,LF,ALFA,ALFA,ALFA                                                                                 //10
};                                                                              

unsigned char reset=1,channel,freq,ocrl,ocrh,fsel,next_bit,msg_count=1,tst_count=1,buf[3]={ALFA,ALFA,ALFA}; 


void AVR_ini(void)
{
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

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=(0<<TOIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<ICIE1) | (0<<OCIE0B) | (0<<TOIE0) | (1<<OCIE0A);

// Global enable interrupts
#asm("sei")

}

static void spi_soft(unsigned char byte)
{
static unsigned char  i;
for (i=8; i; i--)
    {
    if (byte&0x80)	AD9832_SDATA=1;  
    else            AD9832_SDATA=0;
    AD9832_SCLK=0;	               
    #asm("nop")
    byte<<=1;
    AD9832_SCLK=1;
    }
}

void AD9832_write(unsigned  char COMMAND, unsigned char DATA)
{
//C3 C2 C1 C0 A3 A2 A1 A0 D7 D6 D5 D4 D3 D2 D1 D0
AD9832_FSYNC=0;
spi_soft(COMMAND); 		        //14 LSBs
spi_soft(DATA); 				//14 MSBs
AD9832_FSYNC=1;
}


void AD9832_ini(unsigned char i) 
{    
//#asm("cli")
AD9832_write(0b11111000,0b00000000);             //Reset DDS
AD9832_write(0b00110011,AD9832_FREQ[0+(8*i)]);   //Set inital FREQ0 REG 8 H MSBs
AD9832_write(0b00100010,AD9832_FREQ[1+(8*i)]);   //Set inital FREQ0 REG 8 L MSBs
AD9832_write(0b00110001,AD9832_FREQ[2+(8*i)]);   //Set inital FREQ0 REG 8 H LSBs
AD9832_write(0b00100000,AD9832_FREQ[3+(8*i)]);   //Set inital FREQ0 REG 8 L LSBs
AD9832_write(0b00110111,AD9832_FREQ[4+(8*i)]);   //Set inital FREQ1 REG 8 H MSBs
AD9832_write(0b00100110,AD9832_FREQ[5+(8*i)]);   //Set inital FREQ1 REG 8 L MSBs 
AD9832_write(0b00110101,AD9832_FREQ[6+(8*i)]);   //Set inital FREQ1 REG 8 H LSBs
AD9832_write(0b00100100,AD9832_FREQ[7+(8*i)]);   //Set inital FREQ1 REG 8 L LSBs
AD9832_write(0b11000000,0b00000000);             //Go to operational mode
//#asm("sei")
}


unsigned char scan_but(unsigned char fr)
{
static unsigned char i=1;
if (fr==0)                            
    { 
    if (i<2) i++; else i=0;
    delay_ms(200);
    };
return i; 
}

unsigned char scan_mode(unsigned char cn,unsigned char dev,unsigned char lh)
{
static unsigned char f,tf;
if (dev) f=cn*3+1; 
else  {if (lh) f=cn*3+2;  else f=cn*3;}; //012 1 4 7 
if (f!=tf) reset=1; else reset=0;    
tf=f;
return tf;
}

void led_ini(unsigned char cn)
{
switch(cn)
    {
    case 0: LED_4209=0; LED_518=0; LED_490=1; break;
    case 1: LED_4209=0; LED_518=1; LED_490=0; break;
    case 2: LED_4209=1; LED_518=0; LED_490=0; break;
    };
}


void send_symbol(unsigned char symbol)
{
static unsigned char i;
for(i=0; i<7; i++)
    {
    #asm("cli")
    if (symbol&(1<<i))  {fsel=1; ocrh=0x04; ocrl=0x5F;}
    else                {fsel=0; ocrh=0x04; ocrl=0xD5;};
    next_bit=1;
    #asm("sei")
    while(next_bit){};
    }
}


void send_msg(void)
{
static unsigned char i=0,data;

for(i=0; i<TEST_CYCLE; i++) {if (reset) return; send_symbol(0x00); channel=scan_but(BUT_FREQ); freq=scan_mode(channel, BUT_DEV, BUT_LH);};
for(i=0; i<TEST_CYCLE; i++) {if (reset) return; send_symbol(0xFF); channel=scan_but(BUT_FREQ); freq=scan_mode(channel, BUT_DEV, BUT_LH);};  

for(i=0; i<SYNC_CYCLE; i++) {if (reset) return; send_symbol(PHAS); send_symbol(ALFA); channel=scan_but(BUT_FREQ); freq=scan_mode(channel, BUT_DEV, BUT_LH);};

for(i=0; i<78; i++) 
    {
    if (reset) return;
    switch(i)
        {
        case 8: data=NUM[msg_count/10]; break;
        case 9: data=NUM[msg_count%10]; break;
        default: data=MSG0[i];
        }; 
    buf[2]=buf[1]; buf[1]=buf[0]; buf[0]=data;  
    send_symbol(buf[0]);
    send_symbol(buf[2]);
    channel=scan_but(BUT_FREQ); freq=scan_mode(channel, BUT_DEV, BUT_LH);
    };

for(i=0; i<26; i++) 
    {
    if (reset) return;
    if ((freq==1)||(freq==4)||(freq==7)) data=MSG11[i];
    if ((freq==2)||(freq==5)||(freq==8)) data=MSG12[i];
    if ((freq==0)||(freq==3)||(freq==6)) data=MSG13[i];
    buf[2]=buf[1]; buf[1]=buf[0]; buf[0]=data;  
    send_symbol(buf[0]);
    send_symbol(buf[2]);
    channel=scan_but(BUT_FREQ); freq=scan_mode(channel, BUT_DEV, BUT_LH);
    };      

for(i=0; i<64; i++) 
    {
    if (reset) return;
    switch(i)
        {
        case 50: data=NUM[tst_count/10]; break;
        case 51: data=NUM[tst_count%10]; break;
        default: data=MSG2[i];
        };   
    buf[2]=buf[1]; buf[1]=buf[0]; buf[0]=data;  
    send_symbol(buf[0]);
    send_symbol(buf[2]);
    channel=scan_but(BUT_FREQ); freq=scan_mode(channel, BUT_DEV, BUT_LH);
    };         

if (tst_count<99) tst_count++; else tst_count=1; 
if (msg_count<99) msg_count++; else msg_count=1; 
}

interrupt [TIM0_COMPA] void timer0_compa_isr(void)
{
AD9832_FSEL=fsel;
next_bit=0;
TIMSK|=(1<<OCIE1A); 
TIFR|=(1<<OCF1A);  
}

interrupt [TIM1_COMPA] void timer1_compa_isr(void)
{
OCR1AH=ocrh; OCR1AL=ocrl;
TIMSK&=~(1<<OCIE1A);
}

void   main(void)
{
AVR_ini();
while (1)
    {
    if (reset) 
        {
        #asm("cli")
        reset=0;
        tst_count=1;
        buf[0]=ALFA;buf[1]=ALFA;buf[2]=ALFA;
        channel=scan_but(BUT_FREQ); 
        freq=scan_mode(channel, BUT_DEV, BUT_LH); 
        led_ini(channel); 
        AD9832_ini(freq);   
        #asm("sei")
        };
    send_msg();
    }
}


