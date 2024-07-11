/*
 * File:   dashboard.c
 * Author: bharathkumar
 *
 * Created on 25 June, 2024, 2:02 PM
 */



#include <xc.h>
#include "main.h"
#include "matrix_keypad.h"
#include "clcd.h"
#include "ds1307.h"
#include "i2c.h"

#include <stdio.h>





/* function to get current time from RTC and convert to string */
void get_time(void)
{
	clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);

	if (clock_reg[0] & 0x40)
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	else
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	time[2] = ':';
	time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
	time[4] = '0' + (clock_reg[1] & 0x0F);
	time[5] = ':';
	time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
	time[7] = '0' + (clock_reg[2] & 0x0F);
	time[8] = '\0';
}

/* dashboard function */
void dashboard(char key)
{
    //display logic {"ON", "GN", "GR", "G1", "G2", "G3", "G4", "G5", "C "}
    switch(key)
    {
        case MK_SW1 :
            // crash event
            index = 8;
            store_event(time,index,speed); // store event in logs
            break;
        case MK_SW2 :
            // gear change -> increment
            index = index == 8 ? 1 : (index == 7 ? 7: index + 1);
            store_event(time,index,speed); // store event in logs
            break;
        case MK_SW3 :
            // gear change -> decrement
            index = index == 8 ? 1 : (index < 2 ? 1 : index - 1);
            store_event(time,index,speed); // store event in logs
            break;
        default:
            index = index;
    }

    /* display in CLCD */
    sprintf(str, "  %s  %2d",gear[index],speed); // change to string
    clcd_print("TIME     GEAR SP", LINE1(0)); // time, gear, speed
    get_time(); // current time as string
	clcd_print(time, LINE2(0)); // time
    clcd_print(str, LINE2(8)); // gear and speed
    
}

