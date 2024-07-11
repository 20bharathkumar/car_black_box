/*
 * File:   menu.c
 * Author: bharathkumar
 *
 * Created on 25 June, 2024, 2:03 PM
 */


#include <xc.h>
#include "main.h"
#include "matrix_keypad.h"
#include "clcd.h"

/* array of strings for menu items */
char *menu_item[5] = {   "View Log       ", 
                        "Clear Log      ", 
                        "Download Log   ",
                        "Set Time       ", 
                        "Change Password"};

unsigned char item_ind = 0, line = 1;

void menu(char key)
{
    //logic for menu scrolling
    if(key == MK_SW11) // go UP in the menu
    {
        
        if(line == 1)
        {
            item_ind = item_ind == 0? 0: (item_ind - 1);
        }
        line = 1; // current line * is in
        
        
    }
    else if(key == MK_SW12) // go DOWN in the menu
    {
        if(line == 2)
        {
            item_ind = item_ind == 3? 3: (item_ind + 1);
        }
        line = 2; // current line * is in
        
    }
    else if(key == 21) // long press of MK_SW11 // ENTER the current menu option
    {
        /* display the menu enter statement */
        clcd_print("MENU ENTER..    ", LINE1(0));
        menu_f = item_ind + line - 1;
        clcd_print("                ", LINE2(0));
        clcd_print(menu_item[menu_f], LINE2(0));
        main_f = MENU_ENTER;
        
        /*view log start*/
        p = s;
        
        // 1 sec delay
        TMR0ON = 0;
        sec_delay = 20000;
        TMR0 = 6;
        TMR0ON = 1;
        while(sec_delay);
        
        //clear screen
        clcd_print("                ", LINE1(0));        
        clcd_print("                ", LINE2(0));
        
        return;
        
    }
    else if(key == 22 && back_f == 0) // long press MK_SW12 // Go BACK to dashboard
    {
        main_f = DASHBOARD;
        item_ind = 0;
        line = 1;
        return;
    }
    if(key != 22)
    {
        back_f = 0; // reset flag (used to avoid repeated occurrence of go BACK option)
    }
    
    /* display selection of menu option using '*' */
    if(line == 1)
    {
        clcd_print("*",LINE1(0));
        clcd_print(" ",LINE2(0));
    }
    else
    {
        clcd_print(" ",LINE1(0));
        clcd_print("*",LINE2(0));
    }
    /* display the menu option */
    clcd_print(menu_item[item_ind],     LINE1(1));
    clcd_print(menu_item[item_ind + 1], LINE2(1));
}

