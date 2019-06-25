/*
 * File:   spi.c
 * Author: shawn
 *
 * Created on February 11, 2018, 1:47 AM
 */

#include <xc.h>
#include <libpic30.h>
#include "spi.h"

#define RCLK    LATAbits.LATA4 //Data is latched to output on rising edge

void spi1_init(){
    TRISBbits.TRISB2 = 0x00;    //Sets RB2 as output
    TRISBbits.TRISB3 = 0x00;    //Sets RB3 as output
    TRISAbits.TRISA4 = 0x00;    //Sets RA4 as output    
    
    ANSBbits.ANSB2 = 0x00;      //Sets pin as digital output
    ANSBbits.ANSB3 = 0x00;
    
    RPOR1bits.RP2R = 0x07;       //Sets SDO1 to RP2/RB2 
    RPOR1bits.RP3R = 0x08;       //Sets SCLK1 to RP3/RB3 
    
    SPI1CON1Lbits.CKE = 1;      //Sets clock active state to low, idle to high
    SPI1CON1Lbits.DISSDI = 1;   //Disable Master In (SDI1) Pin
                                //Baud rate is Fclock/2    
    SPI1CON1Lbits.MSTEN = 1;    //Master mode enabled
    SPI1CON1Lbits.SPIEN = 1;    //Enables SPI module
    
    RCLK = 0;
}

void spi1_send (unsigned char data){
    SPI1BUFL = data;                 //Data is sent out on SPI(MSB first)
    while(SPI1STATLbits.SPIBUSY);    //Waits for transmit buffer to empty

    SPI1BUFL;                    //Resets SPIBUSY flag
    
    RCLK = 1;                   //Data is latched to output upon rising edge of RCLK
    RCLK = 0;  
}
