/*
 * File:   DMA.c
 * Author: Shawn Nagar
 *
 * Created on August 16, 2018, 6:09 PM
 */


#include "xc.h"
#include "dma.h"

void dma_init(){
    
    //DAM trigger init
    IPC3bits.DMA1IP = 0x01; //DMA1 interrupt priority to 1
    IFS0bits.DMA1IF = 0; //Clear the DMA1 interrupt status flag
    IEC0bits.DMA1IE = 1; //Enable DMA1 interrupts
    
    
    DMACONbits.DMAEN = 1;       //Enables the DMA module
    DMAH = 0x900;               //DMA high address limit
    DMAL = 0x2BB;              //DMA low address limit
    
    DMACONbits.PRSSEL = 0;      //Fixed priority
    

    
    //Channel 1 setup
    DMACH1bits.CHEN = 0;        //Disables Channel 1
    
    DMASRC1 = 0x2BC;            //Source address
    //sine = 0x2BC
    //Square = 0x4BC
    //Triangle = 0x4C2
    //Saw = 0x8C2
    
    DMADST1 = 0x67A;             //Destination Address (LATB)
    DMACNT1 = 256;              //DMA transaction counter for number of words to
                                //be transferred
    
    DMACH1bits.RELOAD = 1;      //Sets Source address, Destination address and 
                                //Transaction counter to original values before     
                                //next transmission
    DMACH1bits.SIZE = 1;        //Sets data size to 1 byte
    
    DMACH1bits.SAMODE = 0b01;   //Source address remains unchanged 00
                                //Set to 0b01 to increment address
    DMACH1bits.DAMODE = 0b11;   //Destination address is the peripheral and remains unchanged
    DMACH1bits.TRMODE = 0b11;   //Repeated Continuous Transfer mode
                                //Will repeat transmission unless Channel is disabled
                                
    DMAINT1bits.CHSEL = 1;   //Trigger sources set to channel 1 DMA interrupt
    
    //DMAINT1bits.DONEIF = 1 if previous transfer completes and 0 if incomplete
    //Use to know if you can change waveform
    
    DMACH1bits.CHEN = 1;        //Enables Channel 1
    DMACH1bits.CHREQ = 1;       //Set to 1 to initiate software trigger
    
    
    
}