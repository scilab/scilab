/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

typedef struct { real r, i; } complex;
typedef long int /* long int */ logical;
typedef short int shortlogical;
typedef char logical1;
typedef signed char integer1;
typedef short integer2;

#endif /*__DEF_H__*/

