/*
 *
 * Copyright (C) 2010 - DIGITEO - Michael Baudin
 * Copyright (C) 2005 - Bruno Pincon
 * Copyright (C) 2002 - Bruno Pincon
 * Copyright (C) 1997, 1999 - Makoto Matsumoto and Takuji Nishimura
 *
 A C-program for MT19937: Integer version (1999/10/28)
*/
/*  genrand() generates one pseudorandom unsigned int (32bit) */
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

/* Any feedback is very welcome. For any question, comments,       */
/* see http://www.math.keio.ac.jp/matumoto/emt.html or email       */
/* matumoto@math.keio.ac.jp                                        */

/* REFERENCE                                                       */
/* M. Matsumoto and T. Nishimura,                                  */
/* "Mersenne Twister: A 623-Dimensionally Equidistributed Uniform  */
/* Pseudo-Random Number Generator",                                */
/* ACM Transactions on Modeling and Computer Simulation,           */
/* Vol. 8, No. 1, January 1998, pp 3--30.                          */


/*
 *  NOTES
 *    slightly modified par Bruno Pincon for inclusion in scilab
 *      - names have changed (for uniformity with the others genators)
 *      - add get state routine
 *      - add a little verif when the state is changed with the simple
 *        procedure
 *
 *     furthers modifications on May 25 2002 :
 *
 *     1/ corrections of the followings :
 *
 *        bug 1 : the complete state was returned at the scilab level
 *                without the index mti. Now the complete state is a
 *                vector of dim 625 with mti as the first component
 *        bug 2 : the set_state doesn't work if the generator was not
 *                initialised => add a is_init var and returned
 *                the state given with the default initialisation.
 *
 *     2/ Following the modif in the new version of this generator I have
 *       changed the simple initialisation (but not put the init via array)
 *
 *     Sept 2005 : fix for bug 1568
 */

#include <math.h>
#include "grand.h"            /* to check prototypes */
#include "sciprint.h"
#include "others_generators.h"
#include "localization.h"

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

static unsigned int mt[N]; /* the array for the state vector  */
static int mti = N;
static int is_init = 0;
static double DEFAULT_SEED = 5489.0;

unsigned long int randmt(void)
{
    unsigned int y;
    static unsigned int mag01[2] = {0x0, MATRIX_A};
    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    if (mti >= N)   /* generate N words at one time */
    {
        int kk;

        if ( ! is_init )
        {
            set_state_mt_simple(DEFAULT_SEED);
        }

        for (kk = 0; kk < N - M; kk++)
        {
            y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
            mt[kk] = mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1];
        }
        for (; kk < N - 1; kk++)
        {
            y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
            mt[kk] = mt[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1];
        }
        y = (mt[N - 1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
        mt[N - 1] = mt[M - 1] ^ (y >> 1) ^ mag01[y & 0x1];

        mti = 0;
    }

    y = mt[mti++];
    y ^= TEMPERING_SHIFT_U(y);
    y ^= TEMPERING_SHIFT_S(y) & TEMPERING_MASK_B;
    y ^= TEMPERING_SHIFT_T(y) & TEMPERING_MASK_C;
    y ^= TEMPERING_SHIFT_L(y);

    return ( y );
}


int set_state_mt_simple(double s)
{
    /*   set the initial state with the simple procedure  */
    unsigned int seed;

    if ( s == floor(s) && 0.0 <= s && s <= 4294967295.0)
    {
        seed = (unsigned int) s;
        mt[0] = seed & 0xffffffff;
        for (mti = 1; mti < N; mti++)
        {
            mt[mti] =  (1812433253UL * (mt[mti - 1] ^ (mt[mti - 1] >> 30)) + mti);
            /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
            /* In the previous versions, MSBs of the seed affect   */
            /* only MSBs of the array mt[].                        */
            /* 2002/01/09 modified by Makoto Matsumoto             */
            mt[mti] &= 0xffffffffUL;   /* for >32 bit machines */
        }
        is_init = 1;
        return ( 1 );
    }
    else
    {
        sciprint(_("Bad seed for mt, must be an int in [0, 2^32-1]\n"));
        return ( 0 );
    }
}


/*
*  Initialization by "set_state_simple_mt()" is an example. Theoretically,
*  there are 2^19937-1 possible states as an initial state.
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

{
    int i, mti_try;

    mti_try = (int) seed_array[0];
    if (mti_try < 1  ||  mti_try > 624)
    {
        sciprint(_("The first component of the mt state mt, must be an int in [1, 624]\n"));
        return ( 0 );
    }
    is_init = 1;
    mti = mti_try;
    for (i = 0; i < N; i++)
    {
        mt[i] = ((unsigned int) seed_array[i + 1]) & 0xffffffff;
    }
    return ( 1 );
}


/*  To return the state at the scilab level  */
void get_state_mt(double state[])
{
    int i;

    if ( ! is_init )
    {
        set_state_mt_simple(DEFAULT_SEED);
    }

    state[0] = (double) mti;
    for (i = 0; i < N; i++)
    {
        state[i + 1] = (double) mt[i];
    }
}
