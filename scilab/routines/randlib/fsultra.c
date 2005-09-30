/* 
FSU - ULTRA	The greatest random number generator that ever was
		or ever will be.  Way beyond Super-Duper.
		(Just kidding, but we think its a good one.)

Authors:	Arif Zaman (arif@stat.fsu.edu) and
		George Marsaglia (geo@stat.fsu.edu).

Date:		27 May 1992

Version:	1.05

Copyright:	To obtain permission to incorporate this program into
		any commercial product, please contact the authors at
		the e-mail address given above or at

		Department of Statistics and
		Supercomputer Computations Research Institute
		Florida State University
		Tallahassee, FL 32306.

See Also:	README		for a brief description
		ULTRA.DOC	for a detailed description

-----------------------------------------------------------------------
*/ 
/*
   File: ULTRA.C

   This is the ULTRA random number generator written entirely in C.

   This may serve as a model for an assembler version of this routine.
   The programmer should avoid simply duplicating and instead use the
   usual assembler features to increase the speed of this routine.

   Especially the subroutine SWB should be replaced by the one
   machine instruction (usually called subtract-with-borrow) that
   is available in almost every hardware.

   For people not familiar with 8086 assembler, it may help to
   consult this when reading the assembler code. This program should
   be a dropin replacement for the assembler versions, but is about
   half as fast.
*/

/* Slight modifications by Bruno Pincon (4 december 2004) for inclusion 
   in scilab and nsp:

   1/ in scilab we use only i32bit output ( renamed here fsultra )
      and  I have deleted the others;

   2/ only one array is now used (swbseed which is renamed
      swb_state) and the xor with the linear congruential generator
      is done only just before the output.

   3/ add a var is_init (to say if the generator is initialised)

   4/ add routine to set/get the state

*/
#include <math.h>             /* to use floor    */
#include "others_generators.h"
#include "../graphics/Math.h" /* to use sciprint */


#define N  37           /* size of table        */
#define N2 24           /* The shorter lag      */

static int is_init=0;  
static long swb_state[N];          /* state of the swb generator */
static int swb_index=N;            /* an index on the swb state */
static int swb_flag;		   /* the carry flag for the SWB generator */
static unsigned long cong_state;   /* state of the congruential generator */

/* for this generator the state seems completly defined by:
      swb_state, swb_index, swb_flag (which define the state of the swb generator)
      cong_state (which defines the state of the congruential generator)
*/

/* those are the default for the simple initialisation routine */
static  double DEFAULT_SEED1= 1234567.0, DEFAULT_SEED2=7654321.0; 




/* SWB is the subtract-with-borrow operation which should be one line
   in assembler code. This should be done by using the hardware s-w-b
   operation in the SWBfill routine (renamed advance_state_swb here).

   What has been done here is to look at the msb of x, y and z=x-y-c.
   Using these three bits, one can determine if a borrow bit is needed
   or not according to the following table:

	msbz=0  msby=0  msby=1          msbz=1  msby=0  msby=1

	msbx=0  0       1               msbx=0  1       1
	msbx=1  0       0               msbx=1  0       1

   PS: note that the definition is very carefully written because the
   calls to SWB have y and z as the same memory location, so y must
   be tested before z is assigned a value.
*/
#define SWB(c,x,y,z) c = (y<0) ? (((z=x-y-c) < 0) || (x>=0)) : (((z=x-y-c) < 0) && (x>=0));

void advance_state_swb()
{ 
  int i;
  /*
   *  The following are the heart of the system and should be
   *  written is assembler to be as fast as possible. It may even make sense
   *  to unravel the loop and simply wirte 37 consecutive SWB operations!
   */
  for (i=0;  i<N2; i++) 
    SWB(swb_flag,swb_state[i+N-N2],swb_state[i],swb_state[i]);
  for (i=N2; i<N;  i++) 
    SWB(swb_flag,swb_state[i  -N2],swb_state[i],swb_state[i]);
  swb_index = 0;
}

/* set_state_fsultra_simple initializes the state from 2 integers  
   
   it defines the constants and fills the swb_state array one bit at
   a time by taking the leading bit of the xor of a shift register
   and a congruential sequence. The same congruential generator continues
   to be used as a mixing generator for the Subtract-with-borrow generator
   to produce the `ultra' random numbers

   Since this is called just once, speed doesn't matter much and it might
   be fine to leave this subroutine coded just as it is.

   PS:	there are quick and easy ways to fill this, but since random number
	generators are really "randomness amplifiers", it is important to
	start off on the right foot. This is why we take such care here.
*/

int set_state_fsultra_simple(double s1, double s2)
{ 
  unsigned long shrgx, tidbits=0;
  int i, j;

  if (    (s1 == floor(s1) && 0.0 <= s1 && s1 <= 4294967295.0)
       && (s2 == floor(s2) && 0.0 <= s2 && s2 <= 4294967295.0) )
    {
      cong_state = ((unsigned long) s1)*2 + 1;
      shrgx = (unsigned long) s2;
      for ( i=0 ; i<N ; i++)
	{
	  for ( j=32 ; j>0 ; j--)
	    { 
	      cong_state = cong_state * 69069;
	      shrgx = shrgx ^ (shrgx >> 15);
	      shrgx = shrgx ^ (shrgx << 17);
	      tidbits = (tidbits>>1) | (0x80000000 & (cong_state^shrgx));
	    }
	  swb_state[i] = tidbits;
	}
      swb_index = 0;
      swb_flag = 0;
      advance_state_swb();  /* pour retrouver la même séquence que ds scilab V3.0 */
      is_init = 1;
      return 1;
    }
  else
    {
      sciprint("\n\r bad seed for fsultra, must be integers in [0, 2^32-1] \n\r");
      return 0;
    }
}

int set_state_fsultra(double *s)
{ 
  double try;
  int i;

  try = s[0];
  if ( floor(try) != try || try < 0.0  ||  try > (double) N)
    {
      sciprint("\n\r the first component of the fsultra state, must be an integer in [0, %d] \n\r",N);
      return 0;
    }
  swb_index = (int) try;

  try = s[1];
  if ( try != 0.0  &&  try != 1.0)
    {
      sciprint("\n\r the second component of the fsultra state, must be 0 or 1 \n\r");
      return 0;
    }
  swb_flag = (int) try;

  try = s[2];
  if ( floor(try) != try  ||  try <= 0 ||  try > 4294967295.0 )
    {
      sciprint("\n\r the third component of the fsultra state, must be an integer in [1, 2^32-1] \n\r");
      return 0;
    }
  cong_state = (unsigned long) try;
 
  /* no verif here ... */
  for (i = 0 ; i < N ; i++) 
    swb_state[i] = (long) (((unsigned long) s[i+3]) & 0xffffffff);

  is_init = 1;
  return 1;
}


/*  to return the state at the scilab level  */
void get_state_fsultra(double s[])
{
  int i;

  if ( ! is_init )
    set_state_fsultra_simple(DEFAULT_SEED1, DEFAULT_SEED2);

  s[0] = (double)  swb_index;
  s[1] = (double)  swb_flag;
  s[2] = (double)  cong_state;
  for (i = 0 ; i < N ; i++) 
    s[i+3] = (double) (unsigned long) swb_state[i];
}

unsigned long fsultra()
{
  if (swb_index >= N)  /* generate N words at one time */
    { 
      if ( ! is_init )
	set_state_fsultra_simple(DEFAULT_SEED1, DEFAULT_SEED2);
      else
	advance_state_swb();
    }
  return (swb_state[swb_index++] ^ (cong_state = cong_state * 69069));
}
