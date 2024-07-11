/*
 * File:   password.c
 * Author: bharathkumar
 *
 * Created on 25 June, 2024, 2:04 PM
 */

#include <xc.h>
#include "main.h"
#include "matrix_keypad.h"
#include "clcd.h"


#define PW_N 3



//unsigned char pw[LEN];
unsigned char attempt = 1; // current no. of attempt
//unsigned char index = 0; // position of character in the password array
unsigned char try[LEN]; // buffer to save input
unsigned char flag; // flag flipping every 0.5 second
unsigned short int block_time;
unsigned char ind; // index position for password buffer

/* function to read input password */
void addtoarray(unsigned char key)
{
    if(key != ALL_RELEASED)
    {
        /* reset password entry time (3 sec) when a key is pressed */
        TMR0ON = 0; // timer stop
        wait = WAIT_TIME; // set for 3 sec
        TMR0 = 6;
        TMR0ON = 1; // timer start

        
        try[ind] = key; // save the key to a buffer
        
        clcd_print("*", LINE2(ind)); 
        
        ind++; // goto next position
    }
}

/* function to check whether the password is matching */
unsigned char check_password(void)
{
    for(ind = 0; ind < LEN; ind++)
    {
        if(try[ind] != pw[ind]) // entered password buffer and original password
        {
            return 0; // not matching // failed
        }
    }
    ind = 0; // reset to the starting position
    return 1; // matching password // success
}

/* password function */
void password(char key)
{
    //password logic
    if(key == 33) // check main.c 
    {
        ind = 0; // reset password to the first letter
        return;
    }
    
    
    if(attempt <= PW_N) // no. of attempts that can use to input password is 5
    {

        clcd_print("Enter Password : ", LINE1(0));    
       
        /* blink cursor */
        if(flag)
        {
            clcd_print("_", LINE2(ind));
        }
        else
        {
            clcd_print(" ", LINE2(ind));                
        }

        addtoarray(key); // save in buffer 
        
        /* After reading 8 characters (when Password is entered)*/
        if(ind == LEN)
        {
            /* check whether the password entered is a match */
            if(check_password())
            {
                /* print SUCCESS and the MESSAGE */
                clcd_print("   MAIN MENU    ", LINE1(0));
                clcd_print("   ---------    ", LINE2(0));
                // 1 sec delay
                TMR0ON = 0; // timer stop
                sec_delay = 20000;
                TMR0 = 6;
                TMR0ON = 1; // timer start
                while(sec_delay); // blocking delay of 1 sec
                           
                main_f = MENU;
                
                clcd_print("*", LINE1(0));
                
                attempt = 1; // change attempt to 1
                return;
            }
            else
            {
                /* print FAILURE */

                /* display to notify the failure of the attempt */
                sprintf(str,"%d", PW_N - attempt); // string for remaining no. of attempts
                clcd_print("Wrong Password ", LINE1(0));
                clcd_print(str, LINE2(0));
                clcd_print(" attempts left ", LINE2(1));

                /* delay for the message to appear */
                TMR0ON = 0; // timer stop
                sec_delay = 20000;
                TMR0 = 6;
                TMR0ON = 1; // timer start
                while(sec_delay); // blocking delay of 1 sec

                /* display for next entry of password*/
                
                attempt++; // increment attempt
                
                if(attempt > PW_N) // when no attempts remaining
                {
                    /* set for count-down 120 seconds */
                    TMR0ON = 0;
                    TMR0 = 6;
                    TMR0ON = 1;
                    block_time = 120;
                }
                clcd_print("                 ", LINE2(0));

                clcd_print("Enter Password : ", LINE1(0));
            }
            ind = 0; // reset index to 0
        }
    }
    else  // when attempts are exhausted without successful password entry
    {


        /*display Failure */
        clcd_print("USER BLOCKED    ", LINE1(0));
        sprintf(str,"For %3d seconds ", block_time); // count-down 120 sec
       
        clcd_print(str, LINE2(0));
        
        if(!block_time) // after count-down
        {
            attempt = 1; // reset attempts
            wait = WAIT_TIME; // reset password entry time to 3 sec
            clcd_print("               ", LINE2(0)); // clear the second line of CLCD
        }
    }
}
