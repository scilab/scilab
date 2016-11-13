/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
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

#ifndef __DEF_H__
#define __DEF_H__

#ifdef integer
#undef integer
#endif
typedef int integer;

typedef double doublereal;

typedef unsigned int uinteger;
typedef char *address;
typedef short int shortint;
typedef float real;

typedef struct
{
    real r, i;
} complex;
typedef long int /* long int */ logical;
typedef short int shortlogical;
typedef char logical1;
typedef signed char integer1;
typedef short integer2;

#endif /*__DEF_H__*/

