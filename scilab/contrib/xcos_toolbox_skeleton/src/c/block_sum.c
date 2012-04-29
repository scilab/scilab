/* This file is released under the 3-clause BSD license. See COPYING-BSD. */

#include <business.h>
#include <scicos_block4.h>

void block_sum(scicos_block* block, scicos_flag flag)
{
    double* y;
    double* u1;
    double* u2;

    if (flag == OutputUpdate)
    {
        // output
        y = (double *) block->outptr[0];

        // input are indexed in order
        u1 = (double *) block->inptr[0];
        u2 = (double *) block->inptr[1];

        // call business layer
        *y = business_sum(*u1, *u2);
    }
}

