/*
 * File:   ecoder.c
 * Author: shawn
 *
 * Created on February 11, 2018, 11:52 PM
 */


#include <xc.h>

#define Sig_SW PORTBbits.RB4     //Switch Signal from encoder

#define left 1
#define right 2

void encoder_init(){
    
    TRISAbits.TRISA0 = 0x01;    //Sets pin as input
    TRISAbits.TRISA1 = 0x01;
    
    ANSAbits.ANSA0 = 0x00;      //Sets RA0 for Digital input  
    ANSAbits.ANSA1 = 0x00;      //Sets RA1 for Digital input
    
}


void encoder_dir(int Sig_A, int Sig_B, int* old_val, int* counter){
    int curr_val = 0;
    
    /*
     * For right direction, encoder pattern: 00, 01, 11, 10
     * For left direction: 00, 10, 11, 01     
     */
    
    curr_val = (Sig_A << 1) + Sig_B; 
    
    if(curr_val != *old_val){
        switch(curr_val){
            case 0b01 :
                if(*old_val == 0b00){
                    (*counter)--;
                }else{
                    (*counter)++;   
                }
                break;
            
            case 0b11 :
                if(*old_val == 0b01){
                    (*counter)--;
                } else{
                    (*counter)++;
                }
                break;
                
            case 0b10 :
                if(*old_val == 0b11){
                    (*counter)--;
                } else {
                    (*counter)++;
                }
                break;
                
            case 0b00:
                if(*old_val == 0b10){
                    (*counter)--;
                } else {
                    (*counter)++;
                }
        }
    }
    
    *old_val = curr_val;

}


