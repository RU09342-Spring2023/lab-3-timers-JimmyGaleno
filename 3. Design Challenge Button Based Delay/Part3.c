/*
 * Part3.c
 *
 *  Created on: Feb 21, 2023
 *      Author: Jimmy Galeno
 *
 *      This system has the LED blinking speed controlled by how long the user holds down a button.
 *      When the program starts the LED blinks at a set speed of around 4 Hz. When the user presses the
 *      button, the program counts for how long it is held, which then changes the speed of the blinking LED.
 *      The other button is then used as a reset to reset the LED blinking speed back to the original.
 *
 */


#include <msp430.h>

void main(){

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    gpioInit();
    timerInit();

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
        PM5CTL0 &= ~LOCKLPM5;

}


void gpioInit(){
    // Initialized the Green LED
    P6OUT &= ~BIT6;                         // Clear P6.6 output latch for a defined power-on state
    P6DIR |= BIT6;                          // Set P6.6 to output direction


    // @ Initialized Button 2.3 and 4.1
    P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
    P2REN |= BIT3;                          // P2.3 pull-up register enable
    P2IES &= ~BIT3;                         // P2.3 Low --> High edge
    P2IE |= BIT3;                           // P2.3 interrupt enabled

    P4OUT |= BIT1;                          // Configure P2.3 as pulled-up
    P4REN |= BIT1;                          // P4.1 pull-up register enable
    P4IES &= ~BIT1;                         // P4.1 Low --> High edge
    P4IE |= BIT1;                           // P4.1 interrupt enabled

}

void timerInit(){
    // Initialize Timer B1 in Continuous Mode using ACLK as the source CLK with Interrupts turned on
    TB1CTL = TBSSEL_1 | MC_2 | TBCLR;           // ACLK, continuous mode, clear TAR

    TB1CCR0 = 50000;                          // Set CCR0 to toggle every 1/2 second

    TB1CCTL0 |= CCIE;                           // Enable CCR0 interrupt
}

