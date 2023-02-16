/*
 * Part2.c
 *
 *  Created on: Feb 16, 2023
 *      Author: Jimmy Galeno
 *
 *      This code will need to change the speed of an LED between 3 different speeds by pressing a button.
 */

#include <msp430.h>

void gpioInit();
void timerInit();

unsigned int t = 50000;                     // t = 50000 clock cycles

void main(){

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    gpioInit();
    timerInit();

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(LPM3_bits | GIE);

}


void gpioInit(){
    // @TODO Initialize the Red or Green LED
    P6OUT &= ~BIT6;                         // Clear P6.6 output latch for a defined power-on state
    P6DIR |= BIT6;                          // Set P6.6 to output direction


    // @TODO Initialize Button 2.3
    P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
    P2REN |= BIT3;                          // P2.3 pull-up register enable
    P2IES &= ~BIT3;                         // P2.3 Low --> High edge
    P2IE |= BIT3;                           // P2.3 interrupt enabled


}

void timerInit(){
    // @TODO Initialize Timer B1 in Continuous Mode using ACLK as the source CLK with Interrupts turned on
    TB1CTL = TBSSEL_1 | MC_2 | TBCLR;           // ACLK, continuous mode, clear TAR

    TB1CCR0 = 50000;                          // Set CCR0 to toggle every 1/2 second

    TB1CCTL0 |= CCIE;                           // Enable CCR0 interrupt
}


/*
 * INTERRUPT ROUTINES
 */

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    // @TODO Remember that when you service the GPIO Interrupt, you need to set the interrupt flag to 0.
    P2IFG &= ~BIT3; // clear P2.3 interrupt flag

    // @TODO When the button is pressed, you can change what the CCR0 Register is for the Timer. You will need to track what speed you should be flashing at.

    // Update CCR0 value based on speed
          if (t == 50000) {                   // starts with inital clock of 50000
              t = 20000;
          } else if (t == 20000) {            // speeds up the cycle to 20000 (first phase when button pressed)
              t = 15000;
          } else if (t == 15000) {            // speeds up the cycle to 15000 (second phase when button pressed)
              t = 10000;
          }else if (t == 10000) {             // speeds up the cycle to 10000 (third phase when button pressed again)
              t = 5000;
          }else if (t == 5000) {              // speeds up the cycle to 5000 (fourth phase when button pressed again)
              t = 1000;
          }else {
              t = 50000;                        // resets to initial clock of 50000 (when button pressed for the fifth time)
          }
}


// Timer B1 interrupt service routine
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void)
{
    // @TODO You can toggle the LED Pin in this routine and if adjust your count in CCR0.
    P6OUT ^= BIT6;       // toggles green LED
    TB1CCR0 += t;        // offsets the clock

}


