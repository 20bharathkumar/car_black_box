/*
 * File:   change_password.c
 * Author: bharathkumar
 *
 * Created on 25 June, 2024, 2:00 PM
 */


#include <xc.h>
#include "main.h"
#include "matrix_keypad.h"
#include "clcd.h"
#include "timer0.h"
#include "extern_eeprom.h"

unsigned char screen; // screens for old, new and re-enter password
unsigned char ind; // index position
unsigned char try[LEN]; // buffer to save input
unsigned char newpw[LEN]; // buffer to save new password input



void addtoarray(unsigned char key); // defined in password.c
unsigned char check_password(void); // defined in password.c


/* function to read input password */
void addtoarray2(unsigned char key)
{
    if(key != ALL_RELEASED)
    {
               
        newpw[ind] = key; // save the key to new password buffer
        
        clcd_print("*", LINE2(ind));
        
        ind++;
    }
}

/* function to check whether the password is matching */
unsigned char check_newpassword(void)
{
    for(ind = 0; ind < LEN; ind++)
    {
        if(try[ind] != newpw[ind]) // check new-password and re-entered password
        {
            return 0; // not matching // failed
        }
    }
    ind = 0;
    return 1; // matching password // success
}

/* Change Password Function */
void change_pass(char key)
{
    //change password logic
    if(key == 22)
    {
        main_f = MENU;
        back_f = 1;
        screen = 0;
        return;
    }
    
    if(screen == 0) // OLD PASSWORD ENTRY
    {
        clcd_print("ENTER OLD PSWD  ", LINE1(0));
        
        /* blink cursor */
        if(flag)
        {
            clcd_print("_", LINE2(ind));
        }
        else
        {
            clcd_print(" ", LINE2(ind));                
        }

        addtoarray(key);
        /* After reading 8 characters (when Password is entered)*/
        if(ind == LEN)
        {
            /* check whether the password entered is a match */
            if(check_password())
            {
                /* on password matching go to next screen for new password entry */
                
                clcd_print("ENTER NEW PSWD   ", LINE1(0));
                clcd_print("                 ", LINE2(0));
                screen = 1;
                ind = 0;
            }
            else
            {
                /* go BACK to dashboard if password fails */
                main_f = DASHBOARD;
                ind = 0;
                screen = 0;
                return;
            }
        }              
        
    }
    else if(screen == 1) // NEW PASSWORD ENTRY
    {
        /* blink cursor */
        if(flag)
        {
            clcd_print("_", LINE2(ind));
        }
        else
        {
            clcd_print(" ", LINE2(ind));                
        }

        addtoarray2(key);
        /* After reading 8 characters (when Password is entered)*/
        if(ind == LEN)
        {
            clcd_print("REENTER NEW PSWD ", LINE1(0));
            clcd_print("                 ", LINE2(0));
            screen = 2;
            ind = 0;
        }
    }
    else if(screen == 2) // RE-ENTRY OF NEWPASSWORD FOR CONFIRMATION
    {
        /* blink cursor */
        if(flag)
        {
            clcd_print("_", LINE2(ind));
        }
        else
        {
            clcd_print(" ", LINE2(ind));                
        }

        addtoarray(key);
        /* After reading 8 characters (when Password is entered)*/
        if(ind == LEN)
        {
            /* check whether the password entered is a match */
            if(check_newpassword())
            {
                /*save in memory */
                write_external_eeprom(100, newpw[0]);
                write_external_eeprom(101, newpw[1]);
                write_external_eeprom(102, newpw[2]);
                write_external_eeprom(103, newpw[3]);
                
                /* re-assign password */
                pw[0] = newpw[0];
                pw[1] = newpw[1];
                pw[2] = newpw[2];
                pw[3] = newpw[3];
                
                /* display SUCCESS message */
                clcd_print("SUCCESS..        ", LINE1(0));
                clcd_print("PSWD SAVED.      ", LINE2(0));
                
                /* 1 sec delay for the message to appear */
                TMR0ON = 0;
                sec_delay = 20000;
                TMR0 = 6;
                TMR0ON = 1;
                while(sec_delay);

                /* go BACK to the MENU */
                main_f = MENU;
                back_f = 1;
                screen = 0;
                
            }
            else
            {
                /* display FAIL message */
                clcd_print("FAILED!!        ", LINE1(0));
                clcd_print("NOT MATCHING..  ", LINE2(0));

                /* 1 sec delay for the message to appear */
                TMR0ON = 0;
                sec_delay = 20000;
                TMR0 = 6;
                TMR0ON = 1;
                while(sec_delay);

                /* Go BACK to the MENU */
                main_f = MENU;
                back_f = 1;
                screen = 0;
                return;

            }
                
        }
        
        
    }
    
    
    
}