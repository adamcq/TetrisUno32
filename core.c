/* mipslabfunc.c
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson

   Modified by Adam Borgula, Yannick Alfred Knoll

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <stdlib.h>
#include "pic32mx.h"  /* Declarations of system-specific addresses etc */
#include "core.h"  /* Declatations for these labs */

/* Declare a helper function which is local to this file */
static void num32asc( char * s, int ); 

#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)

/* quicksleep:
   A simple function to create a small delay.
   Very inefficient use of computing resources,
   but very handy in some special cases. */
void quicksleep(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

/* tick:
   Add 1 to time in memory, at location pointed to by parameter.
   Time is stored as 4 pairs of 2 NBCD-digits.
   1st pair (most significant byte) counts days.
   2nd pair counts hours.
   3rd pair counts minutes.
   4th pair (least significant byte) counts seconds.
   In most labs, only the 3rd and 4th pairs are used. */
void tick( unsigned int * timep )
{
  /* Get current value, store locally */
  register unsigned int t = * timep;
  t += 1; /* Increment local copy */
  
  /* If result was not a valid BCD-coded time, adjust now */

  if( (t & 0x0000000f) >= 0x0000000a ) t += 0x00000006;
  if( (t & 0x000000f0) >= 0x00000060 ) t += 0x000000a0;
  /* Seconds are now OK */

  if( (t & 0x00000f00) >= 0x00000a00 ) t += 0x00000600;
  if( (t & 0x0000f000) >= 0x00006000 ) t += 0x0000a000;
  /* Minutes are now OK */

  if( (t & 0x000f0000) >= 0x000a0000 ) t += 0x00060000;
  if( (t & 0x00ff0000) >= 0x00240000 ) t += 0x00dc0000;
  /* Hours are now OK */

  if( (t & 0x0f000000) >= 0x0a000000 ) t += 0x06000000;
  if( (t & 0xf0000000) >= 0xa0000000 ) t = 0;
  /* Days are now OK */

  * timep = t; /* Store new value */
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpointer-to-int-cast"
/* display_debug
   A function to help debugging.

   After calling display_debug,
   the two middle lines of the display show
   an address and its current contents.

   There's one parameter: the address to read and display.

   Note: When you use this function, you should comment out any
   repeated calls to display_image; display_image overwrites
   about half of the digits shown by display_debug.
*/   
void display_debug( volatile int * const addr )
{
  display_string( 1, "Addr" );
  display_string( 2, "Data" );
  num32asc( &textbuffer[1][6], (int) addr );
  num32asc( &textbuffer[2][6], *addr );
  display_update();
}
#pragma clang diagnostic pop

uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while(!(SPI2STAT & 1));
	return SPI2BUF;
}

void display_init(void) {
        DISPLAY_CHANGE_TO_COMMAND_MODE;
	quicksleep(10);
	DISPLAY_ACTIVATE_VDD;
	quicksleep(1000000);
	
	spi_send_recv(0xAE);
	DISPLAY_ACTIVATE_RESET;
	quicksleep(10);
	DISPLAY_DO_NOT_RESET;
	quicksleep(10);
	
	spi_send_recv(0x8D);
	spi_send_recv(0x14);
	
	spi_send_recv(0xD9);
	spi_send_recv(0xF1);
	
	DISPLAY_ACTIVATE_VBAT;
	quicksleep(10000000);
	
	spi_send_recv(0xA1);
	spi_send_recv(0xC8);
	
	spi_send_recv(0xDA);
	spi_send_recv(0x20);
	
	spi_send_recv(0xAF);
}

void display_string(int line, char *s) {
	int i;
	if(line < 0 || line >= 4)
		return;
	if(!s)
		return;
	
	for(i = 0; i < 16; i++)
		if(*s) {
			textbuffer[line][i] = *s;
			s++;
		} else
			textbuffer[line][i] = ' ';
}

void display_image(int x, const uint8_t *data) {
	int i, j;
	
	for(i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;

		spi_send_recv(0x22);
		spi_send_recv(i);
		
		spi_send_recv(x & 0xF);
		spi_send_recv(0x10 | ((x >> 4) & 0xF));
		
		DISPLAY_CHANGE_TO_DATA_MODE;
		
		for(j = 0; j < 128; j++)
			spi_send_recv(data[i*128 + j]);
	}
}

void display_update(void) {
	int i, j, k;
	int c;
	for(i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;
		spi_send_recv(0x22);
		spi_send_recv(i);
		
		spi_send_recv(0x0);
		spi_send_recv(0x10);
		
		DISPLAY_CHANGE_TO_DATA_MODE;
		
		for(j = 0; j < 16; j++) {
			c = textbuffer[i][j];
			if(c & 0x80)
				continue;
			
			for(k = 0; k < 8; k++)
				spi_send_recv(font[c*8 + k]);
		}
	}
}

/* Helper function, local to this file.
   Converts a number to hexadecimal ASCII digits. */
static void num32asc( char * s, int n ) 
{
  int i;
  for( i = 28; i >= 0; i -= 4 )
    *s++ = "0123456789ABCDEF"[ (n >> i) & 15 ];
}

/*
 * nextprime
 * 
 * Return the first prime number larger than the integer
 * given as a parameter. The integer must be positive.
 */
#define PRIME_FALSE   0     /* Constant to help readability. */
#define PRIME_TRUE    1     /* Constant to help readability. */
int nextprime( int inval )
{
   register int perhapsprime = 0; /* Holds a tentative prime while we check it. */
   register int testfactor; /* Holds various factors for which we test perhapsprime. */
   register int found;      /* Flag, false until we find a prime. */

   if (inval < 3 )          /* Initial sanity check of parameter. */
   {
     if(inval <= 0) return(1);  /* Return 1 for zero or negative input. */
     if(inval == 1) return(2);  /* Easy special case. */
     if(inval == 2) return(3);  /* Easy special case. */
   }
   else
   {
     /* Testing an even number for primeness is pointless, since
      * all even numbers are divisible by 2. Therefore, we make sure
      * that perhapsprime is larger than the parameter, and odd. */
     perhapsprime = ( inval + 1 ) | 1 ;
   }
   /* While prime not found, loop. */
   for( found = PRIME_FALSE; found != PRIME_TRUE; perhapsprime += 2 )
   {
     /* Check factors from 3 up to perhapsprime/2. */
     for( testfactor = 3; testfactor <= (perhapsprime >> 1) + 1; testfactor += 1 )
     {
       found = PRIME_TRUE;      /* Assume we will find a prime. */
       if( (perhapsprime % testfactor) == 0 ) /* If testfactor divides perhapsprime... */
       {
         found = PRIME_FALSE;   /* ...then, perhapsprime was non-prime. */
         goto check_next_prime; /* Break the inner loop, go test a new perhapsprime. */
       }
     }
     check_next_prime:;         /* This label is used to break the inner loop. */
     if( found == PRIME_TRUE )  /* If the loop ended normally, we found a prime. */
     {
       return( perhapsprime );  /* Return the prime we found. */
     } 
   }
   return( perhapsprime );      /* When the loop ends, perhapsprime is a real prime. */
} 

/*
 * itoa
 * 
 * Simple conversion routine
 * Converts binary to decimal numbers
 * Returns pointer to (static) char array
 * 
 * The integer argument is converted to a string
 * of digits representing the integer in decimal format.
 * The integer is considered signed, and a minus-sign
 * precedes the string of digits if the number is
 * negative.
 * 
 * This routine will return a varying number of digits, from
 * one digit (for integers in the range 0 through 9) and up to
 * 10 digits and a leading minus-sign (for the largest negative
 * 32-bit integers).
 * 
 * If the integer has the special value
 * 100000...0 (that's 31 zeros), the number cannot be
 * negated. We check for this, and treat this as a special case.
 * If the integer has any other value, the sign is saved separately.
 * 
 * If the integer is negative, it is then converted to
 * its positive counterpart. We then use the positive
 * absolute value for conversion.
 * 
 * Conversion produces the least-significant digits first,
 * which is the reverse of the order in which we wish to
 * print the digits. We therefore store all digits in a buffer,
 * in ASCII form.
 * 
 * To avoid a separate step for reversing the contents of the buffer,
 * the buffer is initialized with an end-of-string marker at the
 * very end of the buffer. The digits produced by conversion are then
 * stored right-to-left in the buffer: starting with the position
 * immediately before the end-of-string marker and proceeding towards
 * the beginning of the buffer.
 * 
 * For this to work, the buffer size must of course be big enough
 * to hold the decimal representation of the largest possible integer,
 * and the minus sign, and the trailing end-of-string marker.
 * The value 24 for ITOA_BUFSIZ was selected to allow conversion of
 * 64-bit quantities; however, the size of an int on your current compiler
 * may not allow this straight away.
 */
#define ITOA_BUFSIZ ( 24 )
char * itoaconv( int num )
{
  register int i, sign;
  static char itoa_buffer[ ITOA_BUFSIZ ];
  static const char maxneg[] = "-2147483648";
  
  itoa_buffer[ ITOA_BUFSIZ - 1 ] = 0;   /* Insert the end-of-string marker. */
  sign = num;                           /* Save sign. */
  if( num < 0 && num - 1 > 0 )          /* Check for most negative integer */
  {
    for( i = 0; i < sizeof( maxneg ); i += 1 )
    itoa_buffer[ i + 1 ] = maxneg[ i ];
    i = 0;
  }
  else
  {
    if( num < 0 ) num = -num;           /* Make number positive. */
    i = ITOA_BUFSIZ - 2;                /* Location for first ASCII digit. */
    do {
      itoa_buffer[ i ] = num % 10 + '0';/* Insert next digit. */
      num = num / 10;                   /* Remove digit from number. */
      i -= 1;                           /* Move index to next empty position. */
    } while( num > 0 );
    if( sign < 0 )
    {
      itoa_buffer[ i ] = '-';
      i -= 1;
    }
  }
  /* Since the loop always sets the index i to the next empty position,
   * we must add 1 in order to return a pointer to the first occupied position. */
  return( &itoa_buffer[ i + 1 ] );
}

char textbuffer[4][16];

const uint8_t font[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 94, 0, 0, 0, 0,
        0, 0, 4, 3, 4, 3, 0, 0,
        0, 36, 126, 36, 36, 126, 36, 0,
        0, 36, 74, 255, 82, 36, 0, 0,
        0, 70, 38, 16, 8, 100, 98, 0,
        0, 52, 74, 74, 52, 32, 80, 0,
        0, 0, 0, 4, 3, 0, 0, 0,
        0, 0, 0, 126, 129, 0, 0, 0,
        0, 0, 0, 129, 126, 0, 0, 0,
        0, 42, 28, 62, 28, 42, 0, 0,
        0, 8, 8, 62, 8, 8, 0, 0,
        0, 0, 0, 128, 96, 0, 0, 0,
        0, 8, 8, 8, 8, 8, 0, 0,
        0, 0, 0, 0, 96, 0, 0, 0,
        0, 64, 32, 16, 8, 4, 2, 0,
        0, 62, 65, 73, 65, 62, 0, 0,
        0, 0, 66, 127, 64, 0, 0, 0,
        0, 0, 98, 81, 73, 70, 0, 0,
        0, 0, 34, 73, 73, 54, 0, 0,
        0, 0, 14, 8, 127, 8, 0, 0,
        0, 0, 35, 69, 69, 57, 0, 0,
        0, 0, 62, 73, 73, 50, 0, 0,
        0, 0, 1, 97, 25, 7, 0, 0,
        0, 0, 54, 73, 73, 54, 0, 0,
        0, 0, 6, 9, 9, 126, 0, 0,
        0, 0, 0, 102, 0, 0, 0, 0,
        0, 0, 128, 102, 0, 0, 0, 0,
        0, 0, 8, 20, 34, 65, 0, 0,
        0, 0, 20, 20, 20, 20, 0, 0,
        0, 0, 65, 34, 20, 8, 0, 0,
        0, 2, 1, 81, 9, 6, 0, 0,
        0, 28, 34, 89, 89, 82, 12, 0,
        0, 0, 126, 9, 9, 126, 0, 0,
        0, 0, 127, 73, 73, 54, 0, 0,
        0, 0, 62, 65, 65, 34, 0, 0,
        0, 0, 127, 65, 65, 62, 0, 0,
        0, 0, 127, 73, 73, 65, 0, 0,
        0, 0, 127, 9, 9, 1, 0, 0,
        0, 0, 62, 65, 81, 50, 0, 0,
        0, 0, 127, 8, 8, 127, 0, 0,
        0, 0, 65, 127, 65, 0, 0, 0,
        0, 0, 32, 64, 64, 63, 0, 0,
        0, 0, 127, 8, 20, 99, 0, 0,
        0, 0, 127, 64, 64, 64, 0, 0,
        0, 127, 2, 4, 2, 127, 0, 0,
        0, 127, 6, 8, 48, 127, 0, 0,
        0, 0, 62, 65, 65, 62, 0, 0,
        0, 0, 127, 9, 9, 6, 0, 0,
        0, 0, 62, 65, 97, 126, 64, 0,
        0, 0, 127, 9, 9, 118, 0, 0,
        0, 0, 38, 73, 73, 50, 0, 0,
        0, 1, 1, 127, 1, 1, 0, 0,
        0, 0, 63, 64, 64, 63, 0, 0,
        0, 31, 32, 64, 32, 31, 0, 0,
        0, 63, 64, 48, 64, 63, 0, 0,
        0, 0, 119, 8, 8, 119, 0, 0,
        0, 3, 4, 120, 4, 3, 0, 0,
        0, 0, 113, 73, 73, 71, 0, 0,
        0, 0, 127, 65, 65, 0, 0, 0,
        0, 2, 4, 8, 16, 32, 64, 0,
        0, 0, 0, 65, 65, 127, 0, 0,
        0, 4, 2, 1, 2, 4, 0, 0,
        0, 64, 64, 64, 64, 64, 64, 0,
        0, 0, 1, 2, 4, 0, 0, 0,
        0, 0, 48, 72, 40, 120, 0, 0,
        0, 0, 127, 72, 72, 48, 0, 0,
        0, 0, 48, 72, 72, 0, 0, 0,
        0, 0, 48, 72, 72, 127, 0, 0,
        0, 0, 48, 88, 88, 16, 0, 0,
        0, 0, 126, 9, 1, 2, 0, 0,
        0, 0, 80, 152, 152, 112, 0, 0,
        0, 0, 127, 8, 8, 112, 0, 0,
        0, 0, 0, 122, 0, 0, 0, 0,
        0, 0, 64, 128, 128, 122, 0, 0,
        0, 0, 127, 16, 40, 72, 0, 0,
        0, 0, 0, 127, 0, 0, 0, 0,
        0, 120, 8, 16, 8, 112, 0, 0,
        0, 0, 120, 8, 8, 112, 0, 0,
        0, 0, 48, 72, 72, 48, 0, 0,
        0, 0, 248, 40, 40, 16, 0, 0,
        0, 0, 16, 40, 40, 248, 0, 0,
        0, 0, 112, 8, 8, 16, 0, 0,
        0, 0, 72, 84, 84, 36, 0, 0,
        0, 0, 8, 60, 72, 32, 0, 0,
        0, 0, 56, 64, 32, 120, 0, 0,
        0, 0, 56, 64, 56, 0, 0, 0,
        0, 56, 64, 32, 64, 56, 0, 0,
        0, 0, 72, 48, 48, 72, 0, 0,
        0, 0, 24, 160, 160, 120, 0, 0,
        0, 0, 100, 84, 84, 76, 0, 0,
        0, 0, 8, 28, 34, 65, 0, 0,
        0, 0, 0, 126, 0, 0, 0, 0,
        0, 0, 65, 34, 28, 8, 0, 0,
        0, 0, 4, 2, 4, 2, 0, 0,
        0, 120, 68, 66, 68, 120, 0, 0,
};

/*
 * This seed functions is written by
 *   Alex Diaz
 *   Johan Edman
 *
 * under MIT license
 * Copyright (c) 2018 JEDMEX Productions
 * Source: https://github.com/EdmanJohan/UnoShooter/blob/5e226bc947b0ca6ac4fc512e967de429b8bd0726/standard.c
 */
unsigned int seed() {
    int n = 8;
    unsigned int seed = n;
    for (; n > 0; n--) {
        /* Start sampling, wait until conversion is done */
        AD1CON1 |= (0x2);
        while (!(AD1CON1 & (0x2)))
            seed ^= ADC1BUF0 + TMR2;
        while (!(AD1CON1 & 0x1))
            seed ^= ADC1BUF0 + TMR2;

        /* Get the analog value */
        seed ^= ADC1BUF0 + TMR2;
    }
    return seed;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp"
int randint(int max) {
    srand(seed()); //TODO: there may be a better place
    return rand() % (max + 1);
}
#pragma clang diagnostic pop