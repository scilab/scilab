/* A C-program for MT19937: Integer version (1999/10/28)          */
/*  genrand() generates one pseudorandom unsigned integer (32bit) */
/* which is uniformly distributed among 0 to 2^32-1  for each     */
/* call. sgenrand(seed) sets initial values to the working area   */
/* of 624 words. Before genrand(), sgenrand(seed) must be         */
/* called once. (seed is any 32-bit integer.)                     */
/*   Coded by Takuji Nishimura, considering the suggestions by    */
/* Topher Cooper and Marc Rieffel in July-Aug. 1997.              */

/* This library is free software under the Artistic license:       */
/* see the file COPYING distributed together with this code.       */
/* For the verification of the code, its output sequence file      */
/* mt19937int.out is attached (2001/4/2)                           */

/* Copyright (C) 1997, 1999 Makoto Matsumoto and Takuji Nishimura. */
/* Any feedback is very welcome. For any question, comments,       */
/* see http://www.math.keio.ac.jp/matumoto/emt.html or email       */
/* matumoto@math.keio.ac.jp                                        */

/* REFERENCE                                                       */
/* M. Matsumoto and T. Nishimura,                                  */
/* "Mersenne Twister: A 623-Dimensionally Equidistributed Uniform  */
/* Pseudo-Random Number Generator",                                */
/* ACM Transactions on Modeling and Computer Simulation,           */
/* Vol. 8, No. 1, January 1998, pp 3--30.                          */


/* slightly modified par Bruno Pincon for inclusion in scilab */
#include <math.h>
#include "../graphics/Math.h" /* to use sciprint */
#include "grand.h"            /* to check prototypes */

int set_state_mt_simple(double s);


/* Period parameters */  
#define N 624
#define M 397
#define MATRIX_A 0x9908b0df   /* constant vector a */
#define UPPER_MASK 0x80000000 /* most significant w-r bits */
#define LOWER_MASK 0x7fffffff /* least significant r bits */

/* Tempering parameters */   
#define TEMPERING_MASK_B 0x9d2c5680
#define TEMPERING_MASK_C 0xefc60000
#define TEMPERING_SHIFT_U(y)  (y >> 11)
#define TEMPERING_SHIFT_S(y)  (y << 7)
#define TEMPERING_SHIFT_T(y)  (y << 15)
#define TEMPERING_SHIFT_L(y)  (y >> 18)

static unsigned long mt[N]; /* the array for the state vector  */
static int mti=N+1; /* mti==N+1 means mt[N] is not initialized */


double randmt()
{
    unsigned long y;
    static unsigned long mag01[2]={0x0, MATRIX_A};
    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    if (mti >= N) { /* generate N words at one time */
        int kk;

        if (mti == N+1)     /* if set_state_mt_simple() not been called, */
            set_state_mt_simple(4357.0); /* a default initial seed is used   */

        for (kk=0;kk<N-M;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1];
        }
        for (;kk<N-1;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1];
        }
        y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
        mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1];

        mti = 0;
    }
  
    y = mt[mti++];
    y ^= TEMPERING_SHIFT_U(y);
    y ^= TEMPERING_SHIFT_S(y) & TEMPERING_MASK_B;
    y ^= TEMPERING_SHIFT_T(y) & TEMPERING_MASK_C;
    y ^= TEMPERING_SHIFT_L(y);

    return ( (double)y * 2.3283064365386963e-10 ); /* reals: [0,1)-interval */
}

/* initializing the array with a NONZERO seed */
int set_state_mt_simple(double s)
{
  /*   setting initial seeds to mt[N] using
   *   the generator Line 25 of Table 1 in
   *   [KNUTH 1981, The Art of Computer Programming
   *   Vol. 2 (2nd Ed.), pp102]                 
   *
   *   Au niveau de l'interface verifier que s est bien un entier
   */
  unsigned long seed;

  if ( s == floor(s) && 1.0 <= s && s <= 4294967295.0)
    {
      seed = (unsigned long) s;
	mt[0]= seed & 0xffffffff;
      for (mti=1; mti<N; mti++)
        mt[mti] = (69069 * mt[mti-1]) & 0xffffffff;
      mti=N;   
      return ( 1 );
    }
  else
    {
      sciprint("\n\r bad seed for mt : must be an integer with : 1 <= s <= 4294967295 \n\r");
      return ( 0 );
    }
}


/* 
 *  Initialization by "set_state_simple_mt()" is an example. Theoretically,
 *  there are 2^19937-1 possible states as an intial state.           
 *   This function allows to choose any of 2^19937-1 ones.            
 *   Essential bits in "seed_array[]" is following 19937 bits:        
 *      (seed_array[0]&UPPER_MASK), seed_array[1], ..., seed_array[N-1].
 *      (seed_array[0]&LOWER_MASK) is discarded.                         
 *
 *   Theoretically,                                                   
 *      (seed_array[0]&UPPER_MASK), seed_array[1], ..., seed_array[N-1] 
 *   can take any values except all zeros.                            
 */

int set_state_mt(double seed_array[])

     /* pas de verif concernant la condition precedente ... */ 
{
    int i;

    for (i=0;i<N;i++) 
      mt[i] = (unsigned long) seed_array[i];
    mti=N;
    return ( 1 );
}


/*  Pour ramener l'etat au niveau scilab  */
void get_state_mt(double state[])
{
    int i;

    for (i=0;i<N;i++) 
      state[i] = (double) mt[i];
}










