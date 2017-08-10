/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/
/*--------------------------------------------------------------------------*/
#include "configvariable.hxx"

extern "C" {
#include "sci_malloc.h"
#include "grand.h"
#include "clcg4.h"
#include "others_generators.h"
}

#define NbGenInScilab 6
/*--------------------------------------------------------------------------*/
// clcg4 must be called with the virtual generator number
unsigned long int clcg4_with_gen(void)
{
    int current_clcg4 = ConfigVariable::getCurrentClcg4();
    return clcg4(current_clcg4);
}
/*--------------------------------------------------------------------------*/
double C2F(ranf)(void)
{
    //  pointers onto the generators func
    unsigned long int (*gen[NbGenInScilab])() = {randmt, kiss, clcg4_with_gen, clcg2, urandc};

    int current_gen = ConfigVariable::getCurrentBaseGen();

    // the factors (1/(RngMaxInt+1)) to get reals in [0,1) :
    double factor[NbGenInScilab] =
    {
        2.3283064365386963e-10,  // mt
        2.3283064365386963e-10,  // kiss
        4.6566128752457969e-10,  // clcg4
        4.6566130595601735e-10,  // clcg2
        4.6566128730773926e-10   // urand
    };

    // random deviate from U[0,1)
    return ((double)gen[current_gen]() * factor[current_gen]);
}
/*--------------------------------------------------------------------------*/
double ignlgi(void)
{
    //  pointers onto the generators func
    unsigned long int (*gen[NbGenInScilab])() = {randmt, kiss, clcg4_with_gen, clcg2, urandc};

    int current_gen = ConfigVariable::getCurrentBaseGen();

    // random deviate from Ui[0,RngMaxInt] (direct output of the current gen)
    return ((double)gen[current_gen]());
}
/*--------------------------------------------------------------------------*/
double C2F(ignuin)(double *a, double *b)
{
    /*  random deviate from Ui[a,b]
    *  it is assumed that : (i)  a and b are integers (stored in double)
    *                       (ii) b-a+1 <= RngMaxInt[current_gen]
    *  (these verif are done at the calling level)
    *
    *  We use the classic method with a minor difference : to choose
    *  uniformly an int in [a,b] (ie d=b-a+1 numbers) with a generator
    *  which provides uniformly integers in [0,RngMaxInt] (ie m=RngMaxInt+1
    *  numbers) we do the Euclidian division :
    *                                           m = q d + r,   r in [0,d-1]
    *
    *  and accept only numbers l in [0, qd-1], then the output is k = a + (l mod d)
    *  (ie numbers falling in [qd , RngMaxInt] are rejected).
    *  The problem is that RngMaxInt is 2^32-1 for mt and kiss so that RngMaxInt+1 = 0
    *  with the 32 bits unsigned int arithmetic. So in place of rejected r
    *  numbers we reject r+1 by using RngMaxInt in place of m. The constraint is
    *  then that (b-a+1) <= RngMaxInt and if we doesn't want to deal we each generator
    *  we take (b-a+1) <= Min RngMaxInt =  2147483561 (clcg2)
    */

    // all the generators provided integers in [0, RngMaxInt] :
    unsigned long RngMaxInt[NbGenInScilab] =
    {
        4294967295ul,  // mt
        4294967295ul,  // kiss
        2147483646ul,  // clcg4
        2147483561ul,  // clcg2
        2147483647ul   // urand
    };

    int current_gen = ConfigVariable::getCurrentBaseGen();

    unsigned long k, d = (unsigned long)((*b - *a) + 1), qd;

    if (d == 1)
    {
        return (*a);
    }

    qd = RngMaxInt[current_gen] - RngMaxInt[current_gen] % d;

    do
    {
        k = (unsigned long)ignlgi();
    }
    while (k >= qd);

    return (*a + (double)(k % d));
}

