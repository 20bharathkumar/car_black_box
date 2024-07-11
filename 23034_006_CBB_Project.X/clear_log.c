/*
 * File:   clear_log.c
 * Author: bharathkumar
 *
 * Created on 25 June, 2024, 2:01 PM
 */



#include <xc.h>
#include "main.h"
#include "matrix_keypad.h"
#include "clcd.h"
#include "timer0.h"


void clear_log(char key)
{
    //logic for clear
    
    s=0; // reset event log start position (to first memory)
    e = -1; // reset event log end position to -1 to show logs is empty
    p = s; // make present log event to start position
    full_f = 0; // make event log memory full status to zero
    
    /* Display logs cleared */
    clcd_print("  LOGS CLEARED      ", LINE1(0));
    clcd_print("  ------------      ", LINE2(0));
    
    /* 1 sec delay for the message to appear */
    TMR0ON = 0;
    sec_delay = 20000;
    TMR0 = 6;
    TMR0ON = 1;
    while(sec_delay);

    /* Go BACK to the MENU */
    main_f = MENU;
    back_f = 1;
    
}