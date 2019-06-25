/*
 * File:   main.c
 * Author: shawn
 *
 * Created on February 10, 2018, 10:39 PM
 */

#include <xc.h>
#include <p24FJ256GA702.h>
#include "spi.h"
#include "lcd.h"
#include "encoder.h"
#include "timer.h"
#include "dma.h"

//FWDT
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bits (WDT and SWDTEN disabled)
// FOSC
#pragma config FNOSC = FRCPLL           // Oscillator Source Selection (Internal Fast RC (FRC))
#pragma config PLLMODE = PLL6X          // PLL Mode Selection (4x PLL selected)
#pragma config IESO = OFF               // Two-speed Oscillator Start-up Enable bit (Start up with user-selected oscillator source)
#pragma config POSCMD = NONE            // Primary Oscillator Mode Select bits (Primary Oscillator disabled)
#pragma config OSCIOFCN = ON            // OSC2 Pin Function bit (OSC2 is I/O)
#pragma config SOSCSEL = OFF            // SOSC Power Selection Configuration bits (Digital (SCLKI) mode)
#pragma config PLLSS = PLL_FRC          // PLL Secondary Selection Configuration bit (PLL is fed by the on-chip Fast RC (FRC) oscillator)
#pragma config IOL1WAY = ON             // Peripheral pin select configuration bit (Allow only one reconfiguration)
#pragma config FCKSM = CSDCMD           // Clock Switching Mode bits (Both Clock switching and Fail-safe Clock Monitor are disabled

int Sig_A = 0; //Signal from encoder pinA
int Sig_B = 0;
int encoder_read = 0; //Flag is set when encoder is polled

// 10 bit DAC
const int sine_wave[256] = {128,	131,	134,	137,	140,	143,	146,
149,	152,	155,	158,	162,	165,	167,	170,	173,	176,	
179,	182,	185,	188,	190,	193,	196,	198,	201,	203,	
206,	208,	211,	213,	215,	218,	220,	222,	224,	226,	
228,	230,	232,	234,	235,	237,	238,	240,	241,	243,	
244,	245,	246,	248,	249,	250,	250,	251,	252,	253,	
253,	254,	254,	254,	255,	255,	255,	255,	255,	255,	
255,	254,	254,	254,	253,	253,	252,	251,	250,	250,	
249,	248,	246,	245,	244,	243,	241,	240,	238,	237,	
235,	234,	232,	230,	228,	226,	224,	222,	220,	218,	
215,	213,	211,	208,	206,	203,	201,	198,	196,	193,	
190,	188,	185,	182,	179,	176,	173,	170,	167,	165,	
162,	158,	155,	152,	149,	146,	143,	140,	137,	134,	
131,	128,	124,	121,	118,	115,	112,	109,	106,	103,	
100,	97,	93,	90,	88,	85,	82,	79,	76,	73,	70,	67,	65,	62,	59,	57,	54,	52,	49,	
47,	44,	42,	40,	37,	35,	33,	31,	29,	27,	25,	23,	21,	20,	18,	17,	15,	14,	12,	11,	
10,	9,	7,	6,	5,	5,	4,	3,	2,	2,	1,	1,	1,	0,	0,	0,	0,	0,	0,	0,	
1,	1,	1,	2,	2,	3,	4,	5,	5,	6,	7,	9,	10,	11,	12,	14,	15,	17,	18,	20,	
21,	23,	25,	27,	29,	31,	33,	35,	37,	40,	42,	44,	47,	49,	52,	54,	57,	59,	62,	65,	
67,	70,	73,	76,	79,	82,	85,	88,	90,	93,	97,	100,	103,	106,	109,	112,
115,	118,	121,	124};

const int square_wave[3] = {0,255,0};

const int triangle_wave[512] = {0,	2,	4,	6,	8,	10,	12,	14,	16,	18,	20,	22,	
24,	26,	28,	30,	32,	34,	36,	38,	40,	42,	44,	46,	48,	50,	52,	54,	56,	58,	60,	62,	
64,	66,	68,	70,	72,	74,	76,	78,	80,	82,	84,	86,	88,	90,	92,	94,	96,	98,	100,	
102,	104,	106,	108,	110,	112,	114,	116,	118,	120,	
122,	124,	126,	128,	130,	132,	134,	136,	138,	140,	
142,	144,	146,	148,	150,	152,	154,	156,	158,	160,	
162,	164,	166,	168,	170,	172,	174,	176,	178,	180,	
182,	184,	186,	188,	190,	192,	194,	196,	198,	200,	
202,	204,	206,	208,	210,	212,	214,	216,	218,	220,	
222,	224,	226,	228,	230,	232,	234,	236,	238,	240,	
242,	244,	246,	248,	250,	252,	254,	254,	252,	250,	
248,	246,	244,	242,	240,	238,	236,	234,	232,	230,	
228,	226,	224,	222,	220,	218,	216,	214,	212,	210,	
208,	206,	204,	202,	200,	198,	196,	194,	192,	190,	
188,	186,	184,	182,	180,	178,	176,	174,	172,	170,	
168,	166,	164,	162,	160,	158,	156,	154,	152,	150,	
148,	146,	144,	142,	140,	138,	136,	134,	132,	130,	
128,	126,	124,	122,	120,	118,	116,	114,	112,	110,	
108,	106,	104,	102,	100,	98,	96,	94,	92,	90,	88,	86,	84,	82,	80,	
78,	76,	74,	72,	70,	68,	66,	64,	62,	60,	58,	56,	54,	52,	50,	48,	46,	44,	42,	40,	
38,	36,	34,	32,	30,	28,	26,	24,	22,	20,	18,	16,	14,	12,	10,	8,	6,	4,	2,	0};

const int sawtooth_wave[256] = {0,	1,	2,	3,	4,	5,	6,	7,	8,	9,	10,	11,	
12,	13,	14,	15,	16,	17,	18,	19,	20,	21,	22,	23,	24,	25,	26,	27,	28,	29,	30,	31,	
32,	33,	34,	35,	36,	37,	38,	39,	40,	41,	42,	43,	44,	45,	46,	47,	48,	49,	50,	51,	
52,	53,	54,	55,	56,	57,	58,	59,	60,	61,	62,	63,	64,	65,	66,	67,	68,	69,	70,	71,	
72,	73,	74,	75,	76,	77,	78,	79,	80,	81,	82,	83,	84,	85,	86,	87,	88,	89,	90,	91,	
92,	93,	94,	95,	96,	97,	98,	99,	100,	101,	102,	103,	104,	105,	
106,	107,	108,	109,	110,	111,	112,	113,	114,	115,	
116,	117,	118,	119,	120,	121,	122,	123,	124,	125,	
126,	127,	128,	129,	130,	131,	132,	133,	134,	135,	
136,	137,	138,	139,	140,	141,	142,	143,	144,	145,	
146,	147,	148,	149,	150,	151,	152,	153,	154,	155,	
156,	157,	158,	159,	160,	161,	162,	163,	164,	165,	
166,	167,	168,	169,	170,	171,	172,	173,	174,	175,	
176,	177,	178,	179,	180,	181,	182,	183,	184,	185,	
186,	187,	188,	189,	190,	191,	192,	193,	194,	195,	
196,	197,	198,	199,	200,	201,	202,	203,	204,	205,	
206,	207,	208,	209,	210,	211,	212,	213,	214,	215,	
216,	217,	218,	219,	220,	221,	222,	223,	224,	225,	
226,	227,	228,	229,	230,	231,	232,	233,	234,	235,	
236,	237,	238,	239,	240,	241,	242,	243,	244,	245,	
246,	247,	248,	249,	250,	251,	252,	253,	254,	255};




int main(void) {
    
    spi1_init();
    encoder_init();
    timer1_init();          //used to poll encoder
    timer2_init();          //Used for delays
    lcd_init();
    dma_init();

    
    TRISB = 0x00;   //Sets PORTB as output
    
    //Variable Initializations
    int old_val = 0;        //Previous value of encoder signals
    long int counter = 1;
    long int val = 0;    //Stores waveform values (bits15 ..bits8, 0,0,0,0,0,0,0,0)
    
    lcd_MsgWrite("Hello",1,5);
    lcd_MsgWrite("This works :D",2,4);
    
    while (1) {
        
   //     LATB = ((PORTB<<8)>>8) |  sine_wave[val]<<8;
        
        val = val + counter;
        if(counter >= 16){
            counter = 16;
        }
        
        if(val >= 255){
            val = 0;
        }
        
        if (encoder_read) {
        IEC0bits.T1IE = 0; //Disable Timer1 interrupts 
        encoder_dir(Sig_A, Sig_B, &old_val, &counter);
        IEC0bits.T1IE = 1; //Enable Timer1 interrupts
        encoder_read = 0; //resets encoder read flag
        }
        
         
    }
}

void __attribute__((interrupt(auto_psv))) _T1Interrupt(void) {

        
    Sig_A = PORTAbits.RA0; //Reads input every 10 ms
    Sig_B = PORTAbits.RA1;

    encoder_read = 1; //sets encoder read flag
    IFS0bits.T1IF = 0; //Resets timer1 interrupt flag
}

void __attribute__((interrupt(auto_psv))) _T2Interrupt(void) {

    IFS0bits.T2IF = 0; //Resets timer2 interrupt flag

}

void __attribute__((interrupt(auto_psv))) _DMA1Interrupt(void) {

    DMACH1bits.CHREQ = 1;       //Set to 1 to initiate software trigger
    IFS0bits.DMA1IF = 0; //Clear the DMA1 interrupt status flag

}
