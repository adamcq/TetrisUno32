/* mipslabwork.c

   This file written 2022 by Adam Borgula

 */

#include "pic32mx.h"  /* Declarations of system-specific addresses etc */
#include "core.h"  /* Declatations for these labs */
#include "io.h"
#include "gameinterface.h"
#include "gamevar.h"

/* Written as part of i/o lab: getbtns, getsw, enable_interrupt */

void initIO(void)
{
    // set LEDS as output
    TRISE &= ~0xff;

    // set BUTTONS as inputs
    TRISD = (0x7f << 5) | TRISD;
    TRISF = 0x2 | TRISF;

    // initialize TIMER2
    TMR2 = 0;
    setTimer(1);
    T2CON = 0x8070;
    // initialize TIMER2 INTERRUPTS
    IEC(0) = 1 << 8;
    IPC(2) = 7 << 2;
    enable_interrupts();


}

void setTimer(float speedupFactor) {
    PR2 =  BASELINE_CLOCK/ speedupFactor;



}

/*
 * Initializing pins to read from Analog to Digital
 * TODO remove unnecessary initializations
 */
void init_pin() {
    AD1PCFG = 0xFBFF;
    AD1CON1 = 4 << 8 | 7 << 5;
    AD1CHS = 1 << 17;
    TRISBSET = 1 << 11;
    AD1CON2 = 0;
    AD1CON3SET = 1 << 15;

    /* Set up output pins */

    /* Turn on ADC */
    AD1CON1SET = 1 << 15;
}

// get BUTTONS
int getbtn1(void) {
    return (PORTF >> 1) & 0x1;
}

int getbtn2(void) {
    return  (PORTD >> 5) & 0x1;
}

int getbtn3(void) {
    return  (PORTD >> 6) & 0x1;
}

int getbtn4(void) {
    return  (PORTD >> 7) & 0x1;
}

// get SWITCHES
int getsw(void){
  return (PORTD >> 8) & 0xf;
}