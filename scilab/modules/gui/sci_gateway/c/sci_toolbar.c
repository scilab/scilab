/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
#include "gw_gui.h"
#include "stack-c.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "localization.h"
#include "Toolbar.h"
#include "WindowList.h"
#if _MSC_VER
  #include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
int sci_toolbar(char *fname,unsigned long l)
{
  static int l1,n1,m1;
  
  char *Output = NULL;

  char **param = NULL;
  
  int figNum = -2;

  CheckRhs(1,2);
  CheckLhs(0,1);
  
  /* Figure number */
  if ( GetType(1) == sci_matrix )
    {
      GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
      if (m1*n1 != 1)
        {
          Scierror(999, _("%s: Wrong size for first input argument: Scalar expected.\n"), fname);
          return FALSE;
        }
      figNum = *istk(l1);
      
      if (figNum < -1)
        {
          Scierror(999, _("%s: Wrong value for first input argument: value greater than -2 expected.\n"), fname);
          return FALSE;
        }

      if (figNum != -1) /* Check that the figure exists */
        {
          if (getFigureFromIndex(figNum)==NULL)
            {
              Scierror(999, _("%s: Wrong value for first input argument: 'Graphic Window Number %d' does not exist.\n"), fname, figNum);
              return FALSE;
            }
        }
    }
  else
    {
      Scierror(999, _("%s: Wrong type for first input argument: Double value expected.\n"), fname);
      return FALSE;
    }

  if (Rhs==2) /* New status */
    {
      if ((GetType(2) == sci_strings))
        {
          GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&param);
          if (m1*n1 != 1)
            {
              Scierror(999, _("%s: Wrong size for second input argument: 'on' or 'off' expected.\n"), fname);
              return FALSE;
            }
          
          if ( (strcmp(param[0],"off")==0) || (strcmp(param[0],"on")==0) )
            {
              setToolbarVisible(figNum, strcmp(param[0],"on")==0);
            }
          else
            {
              Scierror(999, _("%s: Wrong value second input argument: 'on' or 'off' expected.\n"), fname);
              return FALSE;
            }
        }
      else
        {
          Scierror(999, _("%s: Wrong type for second input argument: 'on' or 'off' expected.\n"), fname);
          return FALSE;
        }
    }

  /* Returned value */
  if (isToolbarVisible(figNum))
    {
      Output = strdup("on");
    }
  else
    {
      Output = strdup("off");
    }
  
  n1 = 1;
  m1 = (int)strlen(Output);
  CreateVarFromPtr(Rhs+ 1,STRING_DATATYPE,&m1,&n1,&Output);
  LhsVar(1) = Rhs+1;
  C2F(putlhsvar)();	

  if (Output) 
    {
      FREE(Output);
      Output=NULL;
    }

  return TRUE;
}
/*--------------------------------------------------------------------------*/
