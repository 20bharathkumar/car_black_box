
/* Project  : Car - Black Box
 * File     : main.c
 * Author   : Bharathkumar K
 * Admn No  : 23034_004
 * Created on 25 June, 2024, 1:51 PM
 */


#include <xc.h>

#include "adc.h"
#include "clcd.h"
#include "extern_eeprom.h"
#include "isr.h"
#include "main.h"
#include "matrix_keypad.h"
#include "timer0.h"
#include "ds1307.h"
#include "i2c.h"
#include "uart.h"

extern unsigned short int block_time;

inti_config()
{
    
    init_adc(); // speed
    init_clcd(); // display
    init_matrix_keypad(); // gear change and menu operations
    init_i2c(); // 
	init_ds1307(); // RTC clock
    init_uart(); // for download logs to laptop
    
    PEIE = 1;
    init_timer0(); // timer for delays
    GIE = 1;

}

void main(void) {
    
    inti_config();
    
    char key; // to read the switches
    
    main_f = 0;     // initialize main flag as zero
    menu_f = 0;     // initialize menu flag as zero
    e = -1;         // initialize end position of logs as -1 before starting
    
    /*speed initialization*/
    unsigned short adc_reg_val;
    adc_reg_val = read_adc(CHANNEL4);
    speed = adc_reg_val / 10;
    speed = speed > 100 ? 99 : speed; // calculate speed 0 - 99

    get_time(); // gets current time in 'time' string
    store_event(time,index,speed); // store ON event
    
#if 0   
         // to give initial password value to external eeprom 
    write_external_eeprom(100, 11);
    write_external_eeprom(101, 11);
    write_external_eeprom(102, 11);
    write_external_eeprom(103, 11);
#endif    
     
    /* Read the Password from the external memory */
    
    pw[0] = read_external_eeprom(100);
    
    pw[1] = read_external_eeprom(101);
        
    pw[2] = read_external_eeprom(102);
        
    pw[3] = read_external_eeprom(103);
           
    while (1)
    {
        /*
         * get the time 
         * based on switch press change the event
         */
        
        /* read the MKP Switch */
        key = read_switches(LEVEL_CHANGE);
        
        /* Update the speed */
        adc_reg_val = read_adc(CHANNEL4);
        speed = adc_reg_val / 10;
        speed = speed > 100 ? 99 : speed; // speed in range 0 - 99
        
        if(main_f == DASHBOARD)
        {
            dashboard(key); // goto dashboard function
            
            if(key == MK_SW11) // goes to password section
            {
                main_f = PASSWORD;
                clcd_print("                ", LINE1(0));
                clcd_print("                ", LINE2(0));
                TMR0ON = 0; // stop the timer
                wait = WAIT_TIME; // initializes wait as 3 sec
                TMR0 = 6;
                TMR0ON = 1; // start the timer
            }

        }
        else if(main_f == PASSWORD)
        {
            /* refresh the Password from the memory */
            
            pw[0] = read_external_eeprom(100);
    
            pw[1] = read_external_eeprom(101);

            pw[2] = read_external_eeprom(102);

            pw[3] = read_external_eeprom(103);
            
            password(key); // goto password function
            
            if(wait == 0 && !block_time ) // when the 3 sec time expires
            {
                main_f = DASHBOARD;
                // reset the 'ind' (index for password) value to zero (first position)
                password(33); 
            }
        }
        else if(main_f == MENU)
        {
            menu(key); // goto menu function
        }
        else if(main_f == MENU_ENTER) 
        {
            if(menu_f == VIEWLOG)
            {
                view_log(key); // function to view logs
            }
            else if(menu_f == DOWNLOADLOG)
            {
                download_log(); // function to download logs
            }
            else if(menu_f == CLEARLOG)
            {
                clear_log(key); // function to clear the logs
            }
            else if(menu_f == SETTIME)
            {
                settime(key); // function to set the time
            }
            else if(menu_f == CHANGEPASS)
            {
                change_pass(key); // function to change the password and update in memory
            }
        }
        
    }
    return;
}
