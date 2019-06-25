/* 
 * File:   lcd.h
 * Author: shawn
 *
 * Created on February 11, 2018, 11:09 PM
 */

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif

    void lcd_init();
    void lcd_CmdWrite(unsigned char);
    void lcd_DataWrite(unsigned char);
    void lcd_MsgWrite(const char*, int, int);
    void lcd_CurPos(int, int);
    void lcd_Clear();
    void lcd_DispNum(int);
    void delay_ms_20(void);
    void delay_ms_5(void);
    void delay_ms_2(void);

#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

