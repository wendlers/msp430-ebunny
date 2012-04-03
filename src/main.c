/*
 * This file is part of the MSP430 ebunny.
 *
 * Copyright (C) 2012 Stefan Wendler <sw@kaltpost.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include  <msp430.h>
#include <legacymsp430.h>

#define P 	20000
#define D	100
#define M	200	

int dc = 1;
int f  = D;
int pwm = 0;
int mode = 0;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD; 

	P1DIR  = BIT0 + BIT6;	
	P1OUT  = BIT0 + BIT6; 

    P1IES |=  BIT3;
    P1IFG &= ~BIT3;
    P1IE  |=  BIT3;

	TACTL = TASSEL1 + ID_0 + MC_1 + TACLR ;
	TACCTL0 = CCIE;	
	TACCR0 = dc;
	_EINT(); 
	
	for (;;) {}

	return 0;
}

interrupt(PORT1_VECTOR) PORT1_ISR(void)
{
    P1IFG &= ~BIT3;                 		

	if(mode) mode = 0; else mode = 1;
}

interrupt(TIMERA0_VECTOR) Timer_A(void)
{
    if(dc > P - D) f = -D; else if(dc < D) f = D;

	if (pwm  == 0)
	{
			dc += f;
			if(mode) { 
    			P1OUT &= ~BIT0;                            
    			P1OUT |= BIT6;                       
			}
			else {
    			P1OUT &= ~BIT0;                            
    			P1OUT &= ~BIT6;                       
			}
			TACCR0 = P - dc;
  			pwm = 1;
	}
	else 		
    {
			if(mode) { 
				P1OUT |= BIT0;					
				P1OUT &= ~BIT6;	
			}
			else {
				P1OUT |= BIT0;					
				P1OUT |= BIT6;	
			}
    		TACCR0 = dc;
  			pwm = 0;    		
    }
}
