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

#include "ultra.h"

#define N  37           /* size of table        */
#define N2 24           /* The shorter lag      */

long swb32[N],		/* These arrays contian the seed mixed with	*/
     swb16[N], 		/* a congruential. They are used 32, 16, and 8  */
     swb8[N], 		/* bits at a time respectively.			*/

     swbseed[N] = { 0x0D45D69A,	/* The seed for the SWB generator	*/
  0x9DB73B1A, 0xA84604E8, 0x7C5F0CA5, 0xBC0196CE, 0xFF4CB42E, 0x7ACA6BE3,
  0xA9ED2A5A, 0x6405A8F7, 0xAC00D4F8, 0xBDD1FC77, 0x064F9DC5, 0xF10AB737,
  0x781293D3, 0x8410C2D2, 0x1C6587DB, 0x7D8F8F0F, 0xF3DCC4EA, 0xB965F99F,
  0x9A0094D1, 0x65976D1C, 0x09173DC1, 0x8E38B992, 0x84701D44, 0x14F0E1B9,
  0xE8A1EC5F, 0x1E925A12, 0xE77A0B5B, 0xCDB5926E, 0xD16260C8, 0xC917E806,
  0x519076AA, 0x7BF6C21C, 0x808C0C90, 0x3E93C7B8, 0x707D6EA0, 0xF1DB698D};
char	flags=0;		/* The carry flag for the SWB generator	*/
long unsigned congx = 0x1C3a128F;    /* Seed for congruential generator	*/

long	*swb32p;	short	swb32n=0;	/* A counter and a	*/
short	*swb16p;	short	swb16n=0;	/* pointer is kept for	*/
char	*swb8p;		short	swb8n=0;	/* each array.		*/

char     swb1[32];				/* The swb1 is slightly	*/
char    *swb1p;		short	swb1n=0;	/* different.		*/

/*************************************************************************
 * For each of swb32, swb16, swb8 and swb1, there is an array, a counter *
 * and a pointer. The counters count down, the pointers go up. When the  *
 * counter reaches zero, a fill routine is called to refill the array.   *
 ************************************************************************/

/* rinit initializes the constants and fills the seed array one bit at
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

void rinit(long unsigned congy,long unsigned shrgx)
{ short i,j;
  unsigned long  tidbits;
  congx=congy*2+1;
  for (i=0;i<N;i++)
  { for (j=32;j>0;j--)
    { congx = congx * 69069;
      shrgx = shrgx ^ (shrgx >> 15);
      shrgx = shrgx ^ (shrgx << 17);
      tidbits = (tidbits>>1) | (0x80000000 & (congx^shrgx));
    }
    swbseed[i] = tidbits;
  }
  swb32n = swb16n = swb8n = swb1n = 0;
  flags = 0;
}

/* SWB is the subtract-with-borrow operation which should be one line
   in assembler code. This should be done by using the hardware s-w-b
   operation in the SWBfill routine.

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

void SWBfill(long *x)
{ short i;
/*
   The following two lines are the heart of the system and should be
   written is assembler to be as fast as possible. It may even make sense
   to unravel the loop and simply wirte 37 consecutive SWB operations!
*/
  for (i=0;  i<N2; i++) SWB(flags,swbseed[i+N-N2],swbseed[i],swbseed[i]);
  for (i=N2; i<N;  i++) SWB(flags,swbseed[i  -N2],swbseed[i],swbseed[i]);
#ifndef fast
  for (i=0;  i<N;  i++) *(x++) = swbseed[i] ^ (congx = congx * 69069);
#else
  for (i=0;  i<N;  i++) *(x++) = swbseed[i];
#endif
}

long swb32fill(void)
{ swb32p = &swb32[0];
  SWBfill(&swb32[0]);
  swb32n = N-1;
  return *(swb32p++);
}

short swb16fill(void)
{ swb16p = (short *) &swb16[0];
  SWBfill((long *) swb16p);
  swb16n = 2*N-1;
  return *(swb16p++);
}

char swb8fill(void)
{ swb8p = (char *) &swb8[0];
  SWBfill((long *) swb8p);
  swb8n = 4*N-1;
  return *(swb8p++);
}

long  i32bit(void) { return  (swb32n--) ? *(swb32p++) : swb32fill(); }
long  i31bit(void) { return ((swb32n--) ? *(swb32p++) : swb32fill()) & 0x7FFFFFFF; }
short i16bit(void) { return  (swb16n--) ? *(swb16p++) : swb16fill(); }
short i15bit(void) { return ((swb16n--) ? *(swb16p++) : swb16fill()) & 0x7FFF; }
char  i8bit(void)  { return  (swb8n--)  ? *(swb8p++)  : swb8fill(); }
char  i7bit(void)  { return ((swb8n--)  ? *(swb8p++)  : swb8fill())  & 0x7F; }

char swb1fill(void)
{ unsigned long i,j;
  swb1p = &swb1[0];
  swb1n = 31;
  i = i32bit();
  for (j=0;j<32;j++) { swb1[j] = (char) (i&1); i=i>>1; }
  return *(swb1p++);
}

char  i1bit(void) { return (swb1n--)  ? *(swb1p++)  : swb1fill(); }

#define two2neg31  ( (2.0/0x10000) / 0x10000 )
#define two2neg32  ( (1.0/0x10000) / 0x10000 )

float vni(void)
{ long temp;
  temp = i32bit();
  if (temp & 0xFF000000) { return (float) (temp * two2neg31); }
  return (float) ((temp + i32bit() * two2neg32) * two2neg31);
}

float uni(void)
{ long temp;
  temp = i31bit();
  if (temp & 0xFF000000) { return (float) (temp * two2neg31); }
  return (float) ((temp + i32bit() * two2neg32) * two2neg31);
}

double dvni(void)
{ unsigned long temp;
  temp = i32bit();
  return ( (long) i32bit() + temp * two2neg32) * two2neg31; }

double duni(void)
{ unsigned long temp;
  temp = i32bit();
  return ( i31bit() + temp * two2neg32) * two2neg31; }

