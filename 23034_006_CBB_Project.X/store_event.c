/*
 * File:   store_event.c
 * Author: bharathkumar
 *
 * Created on 25 June, 2024, 2:06 PM
 */

#include <xc.h>
#include <stdio.h>
#include "main.h"
#include "extern_eeprom.h"
#include "clcd.h"




/* function to store 10 bytes for one event */
void write_10_bytes(char *ptr, char *time,unsigned char index,unsigned char speed)
{
    //time
    write_external_eeprom(ptr  , time[0]);
    write_external_eeprom(ptr+1, time[1]);
    write_external_eeprom(ptr+2, time[3]);
    write_external_eeprom(ptr+3, time[4]);
    write_external_eeprom(ptr+4, time[6]);
    write_external_eeprom(ptr+5, time[7]);
    //gear
    write_external_eeprom(ptr+6, gear[index][0]);
    write_external_eeprom(ptr+7, gear[index][1]);
    //speed
    char tm[3];
    sprintf(tm,"%d",speed);
    write_external_eeprom(ptr+8, tm[0]);
    write_external_eeprom(ptr+9, tm[1]);
}

void store_event(char *time,unsigned char index,unsigned char speed)
{
    //logic for storing 
    if(full_f == 0) // when the no. of event logs are less than ten (flag full is zero)
    {
        s=0; // start position not changing upto ending reaches 10
        e++; // increment end position
        if(e == 10) // when end position is 10, memory is full
        {
            s = 1; // shift start position by one
            e = 0; // make end position to zero
            full_f = 1; // memory full (10 logs present at a time)
        }
        
        write_10_bytes(e * 10, time, index, speed); // store event to memory
    
    }
    else if(full_f == 1) // when th no. of event logs is more than 9 (full)
    {
        s++; // increment start position
        e++; // increment end position
        s = s==10? 0: s; // reposition event logs start position
        e = e==10? 0: e; // reposition event logs end position
        write_10_bytes(e * 10, time, index, speed); // store event to memory
    }

}