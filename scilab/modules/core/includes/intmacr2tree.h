/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/**************************************************/
/* intmacr2tree.h                                 */
/* Functions used for macr2tree() Scilab function */
/**************************************************/

/* Generic header files */
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "machine.h"
#include "stack-c.h" /* stack-def.h, stack1.h, stack2.h, stack3.h included */
#include "Funtab.h"

/**********************/
/* External functions */
/**********************/

/* Defined in SCI/modules/core/src/c/str2sci.c */
extern void str2sci(char** x, int n, int m); /* Write a C-string in Scilab stack */

/* Defined in SCI/modules/core/src/fortran/itosci.f */
extern void C2F(itosci)(int *intptr, int *nbrows, int *nbcols); /* Write a int in Scilab stack */

/* Defined in SCI/modules/core/src/fortran/dtosci.f */
extern void C2F(dtosci)(double *dbleptr, int *nbrows, int *nbcols); /* Write a double in Scilab stack */

/* Defined in SCI/modules/core/src/fortran/cvname.f */
extern int C2F(cvnamel)(int *id, char *str, int *jobptr, int *str_len);
/* *jobptr==0: Get Scilab codes from C-string */
/* *jobptr==1: Get C-string from Scilab codes */

/* Defined in SCI/modules/core/src/fortran/mklist.f */
extern int C2F(mktlist)(int *nbitems);

/* Some useful definitions unexistant in stack-c.h */
#define idstk(x,y) (C2F(vstk).idstk+(x-1)+(y-1)*nsiz)
#define CvNameL(id,str,jobptr,str_len) C2F(cvnamel)(id,str,jobptr,str_len);
#define CvStr(n,line,str,jobptr,str_len) C2F(cvstr)(n,line,str,jobptr,str_len);

/**********************/
/* Internal functions */
/**********************/

/****************************************************************
 Function name: macr2tree
 Decription:
 Create on Scilab stack a "tree" composed of imbricated lists
 and tlists using Scilab pseudo codes of Scilab macro
 Resulting "tree" is a new representation for Scilab macro
 Particularly useful for M2SCI
 Tree is emuled by a "program" tlist:
  tlist(["program","name","outputs","inputs","statements","nblines"],..
                   macro_name,..
                           list_of_output_variable_tlists,..
                                     list_of_input_variable_tlists,..
                                             list_of_macro_statements,..
                                                           total_number_of_lines_in_macro)
 Input:
  - fname: name of calling function for error message
 Output:
  - No output
 Returned value:
  - 0 if execution succeeds
  - not null if execution fails
****************************************************************/
int C2F(macr2tree) (char *fname, unsigned long fname_len);
