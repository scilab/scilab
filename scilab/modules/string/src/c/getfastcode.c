/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET , Cong WU
 * Copyright (C) Bruno PINÇON
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
#include "getfastcode.h"
#include "core_math.h"
/*--------------------------------------------------------------------------*/
/**
* Scilab not uses ASCII code in internal kernel
* TO DO : Remove this with a new kernel
* To understand Scilab coding style :
* scilab code --> ascii code
*           0 --> 0
*           9 --> 9
*          10 --> a
*          35 --> z
*         -10 --> A
*         -35 --> Z
*/
#define NUMBER_INTERNAL_CHARACTERS 128
static int INTERNAL_CHARACTERS_TABLE_CODES_FOR_SCILAB[NUMBER_INTERNAL_CHARACTERS] =
{
    100, 101, 102, 103, 104, 105, 106, 107, 108, -40,
    110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
    120, 121, 122, 123, 124, 125, 126, 127, 128, 129,
    130, 131, 40, 38, -53, 37, 39, 56, 58, 53,
    41, 42, 47, 45, 52, 46, 51, 48,  0,  1,
    2,  3,  4,  5,  6,  7,  8,  9, 44, 43,
    59, 50, 60, -38, -61, -10, -11, -12, -13, -14,
    -15, -16, -17, -18, -19, -20, -21, -22, -23, -24,
    -25, -26, -27, -28, -29, -30, -31, -32, -33, -34,
    -35, 54, 49, 55, 62, 36, -59, 10, 11, 12,
    13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
    23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
    33, 34, 35, -54, 57, -55, 61, 227
};
/*--------------------------------------------------------------------------*/
int C2F(getfastcode)(unsigned char *c, unsigned long c_len)
{
    return convertAsciiCodeToScilabCode(c[0]);
}
/*--------------------------------------------------------------------------*/
int convertAsciiCodeToScilabCode(unsigned char scilab_char)
{
    int k = (int)scilab_char ;
    if (k < NUMBER_INTERNAL_CHARACTERS)
    {
        return INTERNAL_CHARACTERS_TABLE_CODES_FOR_SCILAB[k];
    }
    else
    {
        return k + 100;
    }
}
/*--------------------------------------------------------------------------*/
unsigned char convertScilabCodeToAsciiCode(int scilab_code)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
