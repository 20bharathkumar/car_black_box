/*
 * File:   view_log.c
 * Author: bharathkumar
 *
 * Created on 25 June, 2024, 2:06 PM
 */


#include <xc.h>
#include "main.h"
#include "matrix_keypad.h"
#include "clcd.h"
#include "extern_eeprom.h"



void view_log(char key)
{
    //logic of view log
    if(key == 22) // long press MK_SW11 to go BACK to menu
    {
        main_f = MENU;
        back_f = 1;
        return;
    }
    
   
    if(e >= 0 && e < 10) // when the logs are not empty
    {
        // string to store event logs values
        char logs[17];
                
        if(key == MK_SW11) // go to PREVIOUS event
        {
            if(p != s)
            {
                p = p == 0? 9: p - 1;
            }
        }
        else if(key == MK_SW12) // go to NEXT event
        {
            if(p != e)
            {
                p = p == 9? 0: p + 1;
            }
            
        }
        
        logs[0] = (char) (p - s + 10)%10 + '0'; // calculates serial number for the event
        
        /* read from memory */
        logs[2] = (char) read_external_eeprom(p*10    );
        logs[3] = (char) read_external_eeprom(p*10 + 1);
        logs[5] = read_external_eeprom(p*10 + 2);
        logs[6] = read_external_eeprom(p*10 + 3);
        logs[8] = read_external_eeprom(p*10 + 4);
        logs[9] = read_external_eeprom(p*10 + 5);
        logs[11] = read_external_eeprom(p*10 + 6);
        logs[12] = read_external_eeprom(p*10 + 7);
        logs[14] = read_external_eeprom(p*10 + 8);
        logs[15] = read_external_eeprom(p*10 + 9);
        
        
        logs[1] = ' ';
        logs[4] = ':';
        logs[7] = ':';
        logs[10] = ' ';
        logs[13] = ' ';
        logs[16] = '\0';
        
        clcd_print("# TIME   GEAR SP", LINE1(0));
        clcd_print(logs,LINE2(0));
        
    }
    else // event log is empty
    {
        clcd_print("  VIEW LOG :    ", LINE1(0));
        clcd_print(" < EMPTY LOG >  ", LINE2(0));
    }
}