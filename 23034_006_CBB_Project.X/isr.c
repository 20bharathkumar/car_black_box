/*
 * File:   isr.c
 * Author: bharathkumar
 *
 * Created on 27 June, 2024, 2:20 PM
 */

#include <xc.h>
#include "main.h"
#include "timer0.h"

unsigned short int block_time;

unsigned long int sec_delay;

void __interrupt() isr(void)
{
	static unsigned short count;
 
	if (TMR0IF)
	{
		TMR0 = TMR0 + 8;
        
        count++;

		
        if(count%10000 == 0)
        {
            flag = !flag;
        }
        if(wait)
        {
            wait--;
        }

        if(sec_delay)
        {
            sec_delay--;
        }
        
        if (count == 20000)
		{
			count = 0;
            if(block_time)
            {
                block_time--;
            }
            
		}
		TMR0IF = 0;
	}
}