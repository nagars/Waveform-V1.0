/*
 * File:   timer.c
 * Author: shawn
 *
 * Created on February 26, 2018, 11:18 PM
 */


#include <xc.h>

void timer1_init() {

    T1CONbits.TCS = 0x00; //timer clock is internal clock (Fosc/2)
    T1CONbits.TGATE = 0x00; //Gated accumulation is disabled
    T1CONbits.TCKPS = 0x03; //Uses pre-scaler TimerClock/256
    TMR1 = 0; //Clears Timer register (start value))
    PR1 = 625; //Loads Period register with value (end value))
    //Allows for 10ms interrupts

    IPC0bits.T1IP = 0x01; //Timer1 interrupt priority to 1
    IFS0bits.T1IF = 0; //Clear the Timer1 interrupt status flag
    IEC0bits.T1IE = 1; //Enable Timer1 interrupts

    T1CONbits.TON = 0x01; //Starts Timer1

}

void timer2_init() {

    T2CONbits.TCS = 0x00;   //timer clock is internal clock (Fosc/2)
    T2CONbits.TGATE = 0x00; //Gated accumulation is disabled
    T2CONbits.TCKPS = 0x03; //Uses pre-scaler TimerClock/256
    TMR2 = 0x00;            //Clears Timer register (start value))
    //PR2 = 0xffff;           //PR2 holds final value

    IPC1bits.T2IP = 0x01;   //Timer2 interrupt priority to 1
    IFS0bits.T2IF = 0;      //Clear the Timer2 interrupt status flag
    IEC0bits.T2IE = 1;      //Enable Timer2 interrupts
    T2CONbits.TON = 0x00;   //Timer2 disabled

}