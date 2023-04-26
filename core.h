#include <stdint.h>

/* utils.h
   Header file for all labs.
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson

   Modified by Adam Borgula, Yannick Alfred Knoll

   Latest update 2015-08-28 by F Lundevall

   For copyright and licensing, see file COPYING */

void enable_interrupts();

/* Declare display-related functions from mipslabfunc.c */

void display_image(int x, const uint8_t *data);
void display_init(void);
void display_string(int line, char *s);
void display_update(void);
uint8_t spi_send_recv(uint8_t data);

/* Declare lab-related functions from mipslabfunc.c */
char * itoaconv( int num );
void loop(void);
int nextprime( int inval );
void quicksleep(int cyc);
void tick( unsigned int * timep );

/* Declare display_debug - a function to help debugging.

   After calling display_debug,
   the two middle lines of the display show
   an address and its current contents.

   There's one parameter: the address to read and display.

   Note: When you use this function, you should comment out any
   repeated calls to display_image; display_image overwrites
   about half of the digits shown by display_debug.
*/
void display_debug( volatile int * const addr );



/* Declare functions written by students.
   Note: Since we declare these functions here,
   students must define their functions with the exact types
   specified in the laboratory instructions. */
/* Written as part of asm lab: delay, time2string */
unsigned int seed(void);
int randint(int max);
void delay(int);
void time2string( char *, int );

void enable_interrupt(void);

/* Declare bitmap array containing font */
extern const uint8_t font[128*8];
/* Declare text buffer for display output */
extern char textbuffer[4][16];