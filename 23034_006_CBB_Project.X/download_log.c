/*
 * File:   download_log.c
 * Author: bharathkumar
 *
 * Created on 25 June, 2024, 2:03 PM
 */

#include <xc.h>
#include <stdio.h>

#include "main.h"
#include "matrix_keypad.h"
#include "clcd.h"
#include "extern_eeprom.h"
#include "uart.h"
#include "timer0.h"

void download_log()
{
    //download log logic
    clcd_print(" DOWNLOADING..  ",LINE1(0));
    clcd_print("                ",LINE2(0));
    
    if (e >= 0 && e < 10) // when event log end position is a valid number
    {
        /* send the logs via UART */       
        uart_puts("\n\r");
        uart_puts("SL NO   DATE       GEAR   SPEED\n\r");
        uart_puts("-----   ----       ----   -----\n\r");
        do
        {
            char str[30];
            sprintf(str,"    %c   %c%c:%c%c:%c%c     %c%c      %c%c  \n\r",
            ((p - s + 10)%10 + '0'),        // SL NO
            /* read from memory */
            read_external_eeprom(p*10    ), // HOUR
            read_external_eeprom(p*10 + 1), // HOUR
            read_external_eeprom(p*10 + 2), // MINUTE
            read_external_eeprom(p*10 + 3), // MINUTE
            read_external_eeprom(p*10 + 4), // SECOND
            read_external_eeprom(p*10 + 5), // SECOND
            read_external_eeprom(p*10 + 6), // GEAR
            read_external_eeprom(p*10 + 7), // GEAR
            read_external_eeprom(p*10 + 8), // SPEED
            read_external_eeprom(p*10 + 9));// SPEED
            
            /* send each string via UART */
            uart_puts(str);
            uart_puts("\n\r");
            
            p = p == 9? 0: p + 1; // update p (present position in event logs)
        }while(p != e+1);
        
        uart_puts("\n\r");
    }
    else 
    {
        /* send via UART that logs are empty */
        uart_puts("\n\r");
        uart_puts("LOGS ARE EMPTY\n\r");
        uart_puts("\n\r");
    }
    
    /* 1 sec delay for the message to appear */
    TMR0ON = 0;
    sec_delay = 20000;
    TMR0 = 6;
    TMR0ON = 1;
    while(sec_delay);

    /* Go BACK to the MENU*/
    back_f = 1;
    main_f = MENU;
    
}
