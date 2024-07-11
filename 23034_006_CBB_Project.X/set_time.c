/*
 * File:   set_time.c
 * Author: bharathkumar
 *
 * Created on 25 June, 2024, 2:05 PM
 */


#include <xc.h>
#include "main.h"
#include "matrix_keypad.h"
#include "clcd.h"
#include "ds1307.h"

unsigned char byte; // buffer to store one byte data

static char once, field;
unsigned char hrr,mnn,scd; 
void settime(char key)
{
    //logic for set time
    
    if(once == 0) // do only once
    {
        /* stop the RTC clock */
        byte = read_ds1307(CNTL_ADDR);  // control register
        byte = byte | 0x80;             // make byte.bit7 = 1;
        write_ds1307(CNTL_ADDR, byte);  // save changes

        /* get seconds */
        byte = read_ds1307(SEC_ADDR);   // bits as 0xxx xxxx
        scd = ((byte >> 4 )& 0x07);
        scd = scd * 10;
        scd = scd + (byte & 0x0F);

        /* get minutes */
        byte = read_ds1307(MIN_ADDR);   // bits as 0xxx xxxx  
        mnn = ((byte >> 4 )& 0x07);
        mnn = mnn * 10;
        mnn = mnn + (byte & 0x0F);

        /* get hours */
        byte = read_ds1307(HOUR_ADDR);  // bits as 00xx xxxx
        hrr = ((byte >> 4 )& 0x03);
        hrr = hrr * 10;
        hrr = hrr + (byte & 0x0F);


        once = 1;
    }
    /* change field when Matrix Keypad Switch 12 is pressed*/
    if(key == MK_SW12)
    {
        field = field == 2? 0: field + 1;
    }
    /* increment current field value when Matrix Keypad Switch 11 is pressed*/
    else if(key == MK_SW11)
    {
        if(field == 0) // seconds field
        {
            scd = scd == 59? 0: scd + 1;
        }
        else if(field == 1) // minutes field
        {
            mnn = mnn == 59? 0: mnn + 1;
        }
        else if(field == 2) // hours field
        {
            hrr = hrr == 23? 0: hrr + 1;
        }
    }
    /* Go BACK to the MENU when Matrix Keypad Switch 12 is pressed for long time*/
    else if(key == 22) // long press MK_SW12
    {
        /* resume the clock RTC */
        byte = read_ds1307(CNTL_ADDR);
        byte = byte & 0x7F; //byte.bb7 = 0;
        write_ds1307(CNTL_ADDR, byte);
        
        /*go back to the MENU*/
        main_f = MENU;
        back_f = 1;
        once = 0;
        return;
    }
    
    /* Save the changes and go BACK to the MENU when Matrix Keypad Switch 11 is pressed for long time*/    
    else if (key == 21) // long press MK_SW11
    {
        //save and exit
        //save seconds
        byte = (((scd / 10) << 4) & 0x70)|((scd % 10) & 0x0F);
        write_ds1307(SEC_ADDR, byte);
        //save minutes
        byte = (((mnn / 10) << 4) & 0x70)|((mnn % 10) & 0x0F);
        write_ds1307(MIN_ADDR, byte);
        //save hours
        byte = (((hrr / 10) << 4) & 0x30)|((hrr % 10) & 0x0F);
        write_ds1307(HOUR_ADDR, byte);
        
        /* resume the clock RTC with new input time */
        byte = read_ds1307(CNTL_ADDR);
        byte = byte & 0x7F;//byte.bb7 = 0;
        write_ds1307(CNTL_ADDR, byte);
        
        /* go BACK to the MENU */
        main_f = MENU;
        back_f = 1;
        once = 0;
        return;
    
    }
    
    /* store changes in time to a string */
    char str[17];
    str[0] = (hrr / 10) + '0';
    str[1] = (hrr % 10) + '0';
    str[2] = ':';
    str[3] = (mnn / 10) + '0';
    str[4] = (mnn % 10) + '0';
    str[5] = ':';
    str[6] = (scd / 10) + '0';
    str[7] = (scd % 10) + '0';
    str[8] = '\0';
    
    if(flag) // to show current field by blinking
    {
        switch(field)
        {
            case 0 :
                str[6] = '_';
                str[7] = '_';
                break;
            case 1 :
                str[3] = '_';
                str[4] = '_';
                break;
            case 2:
                str[0] = '_';
                str[1] = '_';
                break;
        }
    }
    /* display */
    clcd_print("SET TIME     ", LINE1(0));
    clcd_print(str, LINE2(0));
    
}