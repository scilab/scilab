/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2009 - Digiteo - Vincent LIARD
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <math.h>
#include "sci_mem_alloc.h"
#include "gw_signal.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"

/*--------------------------------------------------------------------------*/
extern int C2F(scicorr)(char *id,unsigned long fname_len );
/*--------------------------------------------------------------------------*/
int sci_corr(char *fname,unsigned long fname_len)
{
	C2F(scicorr)(fname,fname_len);
	return 0;
}
/*--------------------------------------------------------------------------*/
/****************************************************************/

/****************************************************************/
// extern void C2F()();
/****************************************************************/
int sci_corr0(char *fname, unsigned long fname_len)
{
  enum {usual, updates, fft} procedure;
  int rows, cols;
  char *procedure_name;
  double argument;

  CheckRhs(2,5);
  CheckLhs(1,2);

  if (GetType(1) == sci_strings) {
    GetRhsVar(1, STRING_DATATYPE, &rows, &cols, procedure_name);
    printf("%s\n", procedure_name);
    return 0;
  }
  else {
    procedure = usual;
  }

  procedure = Rhs - 1; //
  switch (procedure) {
  case usual:
    CheckRhs(2,3);
    printf("usual\n");
    break;
  case updates:
    CheckRhs(3,5);
    printf("updates\n");
    break;
  case fft:
    CheckRhs(4,5);
    printf("fft\n");
    break;
  }
  return 0;
}
