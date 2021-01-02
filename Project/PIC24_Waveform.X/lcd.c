/*
 * File:   lcd.c
 * Author: shawn
 *
 * Created on February 11, 2018, 11:10 PM
 */


#include <xc.h>
#include <libpic30.h>
#include "lcd.h"
#include "spi.h"

#define RS      LATAbits.LATA2        //Register Select for command or data
#define Enable  LATAbits.LATA3        //CMD/DATA Enable

void lcd_init(){    
    
    TRISAbits.TRISA3 = 0x00;    //Sets RA3 as an output
    TRISAbits.TRISA2 = 0x00;    //Sets RA2 as an output
     
    Enable = 0;
    RS = 0;  
    
    lcd_CmdWrite(0x30);         //8bit mode
    delay_ms_20();
    lcd_CmdWrite(0x30);         
    delay_ms_20();
    lcd_CmdWrite(0x30);         
    delay_ms_20();
        
    lcd_CmdWrite(0x38);         //8bit mode
    lcd_CmdWrite(0x10);         //Set Cursor
    lcd_CmdWrite(0x0C);         //Display On, Cursor Off
    lcd_CmdWrite(0x06);         //Auto Increment cursor
    lcd_CmdWrite(0x01);         //Clear display
    
    delay_ms_5();
    
}

void lcd_CmdWrite( unsigned char Cmd){
    
    spi1_send(Cmd);

    Enable = 1;
    Enable = 1;
    Enable = 0;
   
    delay_ms_2();
   
}

void lcd_DataWrite(unsigned char Data){
    Enable = 0;
    RS = 1;
    
    spi1_send(Data);

    Enable = 1;
    Enable = 0;
    
    RS = 0;
    delay_ms_2();    
}

/*****************************************
 * Argument 1: Accepts String to be displayed 
 * Argument 2: line number from 0 through 3
 * Argument 3: Col number from 0 through 19
******************************************/
void lcd_MsgWrite(const char* Msg, int line, int col){
    
    lcd_CurPos(line, col);
    
    int n;
    for(n = 0; Msg[n] != '\0'; n++){
        lcd_DataWrite(Msg[n]);
    }
 
}
/*****************************************
 * Sets cursor to given line and col
 * Accepts line number from 0 through 3
 * Accepts Col number from 0 through 19
******************************************/

void lcd_CurPos(int line, int col){
    if(line == 0){
        lcd_CmdWrite(0x80 + col);
    }else if(line == 1){
        lcd_CmdWrite(0xC0 + col);
    }else if(line == 2){
        lcd_CmdWrite(0x94 + col);
    }else{
        lcd_CmdWrite(0xD4 + col);
    }
}

void lcd_Clear(){
    lcd_CmdWrite(0x01);
    lcd_CmdWrite(0x10); 
}

void lcd_DispNum(int num){  //8 letter number
    
   lcd_DataWrite((num/10000000)+0x30);
   num=num%10000000; 
    
   lcd_DataWrite((num/1000000)+0x30);
   num=num%1000000; 
    
   lcd_DataWrite((num/100000)+0x30);
   num=num%100000; 
    
   lcd_DataWrite((num/10000)+0x30);
   num=num%10000;
 
   lcd_DataWrite((num/1000)+0x30);
   num=num%1000;
 
   lcd_DataWrite((num/100)+0x30);
   num=num%100;
 
   lcd_DataWrite((num/10)+0x30);
 
   lcd_DataWrite((num%10)+0x30);
 
}


 void delay_ms_20 (){      
    TMR2 = 0x00;            //Clears Timer register (start value)) 
    T2CONbits.TCKPS = 0x2; //Uses pre-scaler TimerClock/64
    PR2 = 4000;//PR2 sets final timer val 
    T2CONbits.TON = 0x01;   //Enables timer 2
    
    while(!IFS0bits.T2IF);
    
    T2CONbits.TON = 0x00;   //Disables timer 2
     
 }

  void delay_ms_5 (){      
    TMR2 = 0x00;            //Clears Timer register (start value)) 
    T2CONbits.TCKPS = 0x2; //Uses pre-scaler TimerClock/64
    PR2 = 1000;     //PR2 sets final timer val 
    T2CONbits.TON = 0x01;   //Enables timer 2
    
    while(!IFS0bits.T2IF);
    
    T2CONbits.TON = 0x00;   //Disables timer 2
     
 }
  
   void delay_ms_2 (){      
    TMR2 = 0x00;            //Clears Timer register (start value)) 
    T2CONbits.TCKPS = 0x2; //Uses pre-scaler TimerClock/64
    PR2 = 400;//PR2 sets final timer val 
    T2CONbits.TON = 0x01;   //Enables timer 2
    
    while(!IFS0bits.T2IF);
    
    T2CONbits.TON = 0x00;   //Disables timer 2
     
 }