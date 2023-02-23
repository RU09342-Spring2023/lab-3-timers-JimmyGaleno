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

#define initialtimervalue 100

void gpioInit();
void timerInit();

volatile unsigned int counting = 0;
volatile unsigned long count_timer = 0;
volatile unsigned int button_pressed = 0;

void main() {
  WDTCTL = WDTPW | WDTHOLD;
  gpioInit();
  timerInit();

  // Disable the GPIO power-on default high-impedance mode
  // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;
  __bis_SR_register(LPM3_bits | GIE);


}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void) {
  P2IFG &= ~BIT3;

  if (P2IES & BIT3) {
    // Button pressed
    P6OUT &= ~BIT6;
    counting = 1;
    count_timer = 0;
    button_pressed = 1;
    P2IES &= ~BIT3;
  } else {
    // Button released
    counting = 0;
    P2IES |= BIT3;
  }
}

#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void) {
  P4IFG &= ~BIT1;

  TB1CCR0 = initialtimervalue;
  counting = 0;
  button_pressed = 1;
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void) {
  if (counting) {
    count_timer++;
  }

  TB0CCR0 += 1;
}

#pragma vector=TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void) {
  P6OUT ^= BIT6;

  if (button_pressed) {
    TB1CCR0 += count_timer;
  } else {
    TB1CCR0 += initialtimervalue;
  }
}

void gpioInit(){
    // Initialized the Green LED
    P6OUT &= ~BIT6;                         // Clear P6.6 output latch for a defined power-on state
    P6DIR |= BIT6;                          // Set P6.6 to output direction


    // @ Initialized Button 2.3 and 4.1
    P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
    P2REN |= BIT3;                          // P2.3 pull-up register enable
    P2IES &= ~BIT3;                         // P2.3 Low --> High edge
    P2IE  |= BIT3;                           // P2.3 interrupt enabled

    P4OUT |= BIT1;                          // Configure P2.3 as pulled-up
    P4REN |= BIT1;                          // P4.1 pull-up register enable
    P4IES &= ~BIT1;                         // P4.1 Low --> High edge
    P4IE  |= BIT1;                           // P4.1 interrupt enabled

}

void timerInit(){
    // Initialize Timer B1 in Continuous Mode using ACLK as the source CLK with Interrupts turned on
    TB0CTL = TBSSEL_1 | MC_2 | ID_3;           // ACLK, continuous mode, clear TAR

    TB0CCR0 = 1;                                // Set CCR0 to toggle every 1/2 second

    TB0CCTL0 |= CCIE;                           // Enable CCR0 interrupt

    TB1CTL = TBSSEL_1 | MC_2 | ID_3;           // ACLK, continuous mode, clear TAR

    TB1CCR0 = initialtimervalue;

    TB1CCTL0 |= CCIE;                           // Enable CCR0 interrupt



}





