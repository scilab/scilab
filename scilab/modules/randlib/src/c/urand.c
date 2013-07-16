/*
 *
 * Copyright (C) 2010 - DIGITEO - Michael Baudin
 * Copyright (C) 2004 - Bruno Pincon
 * Copyright (C) 1973 - CLEVE B. MOLER
 * Copyright (C) 1973 - MICHAEL A. MALCOLM
 *
 *   PURPOSE
 *      the basic rand generator of Scilab : s <- (a*s + c) mod m
 *      with :
 *             m = 2^{31}
 *             a = 843314861
 *             c = 453816693
 *
 *      s must be in [0,m-1] when user changes seed with set_state_urand
 *      period = m
 *
 *   NOTES
 *      a/ Rewritten (in C) so as to output integers like all the others
 *         generators (and also to have the same manner to set/get the state)
 *      b/ unsigned int arithmetic must be the classic 32 bits unsigned
 *         arithmetic (ie also is exact modulo 2^32).
 *
 *   URAND, A UNIVERSAL RANDOM NUMBER GENERATOR
 *   BY, MICHAEL A. MALCOLM, CLEVE B. MOLER,
 *   STAN-CS-73-334, JANUARY 1973,
 *   COMPUTER SCIENCE  DEPARTMENT,
 *   School of Humanities and Sciences, STANFORD UNIVERSITY,
 *   ftp://reports.stanford.edu/pub/cstr/reports/cs/tr/73/334/CS-TR-73-334.pdf
 *
 *
 */


#include <math.h>             /* to use floor    */
#include "sciprint.h"
#include "others_generators.h"
#include "localization.h"

static unsigned int s = 0;

unsigned long int urandc(void)
{
    s = 843314861ul * s + 453816693ul;  /* => on obtient ici un resultat modulo 2^32 */

    /* il suffit du test suivant pour obtenir le modulo 2^31 */
    if (s >= 2147483648ul)
    {
        s -= 2147483648ul;
    }

    return ( s );
}

int set_state_urand(double g)
{
    if ( g == floor(g) &&  0 <= g && g <= 2147483647 )
    {
        s = (unsigned int) g;
        return ( 1 );
    }
    else
    {
        sciprint(_("\nBad seed for urand, must be an int in [0,  2147483647]\n"));
        return ( 0 );
    }
}

void get_state_urand(double g[])
{
    g[0] = (double) s;
}
