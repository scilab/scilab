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
#include "ObjectStructure.h"
#include "HandleManagement.h"
#include "GetProperty.h"
#if _MSC_VER
  #include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
int sci_toolbar(char *fname,unsigned long l)
{
  static int stkAdr,nbCol,nbRow;
  
  char *Output = NULL;

  char **param = NULL;
  
  int figNum = -2;

  sciPointObj *pObj = NULL;;

  CheckRhs(1,2);
  CheckLhs(0,1);
  
  /* Figure number */
  if ( GetType(1) == sci_matrix )
    {
      GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&nbRow,&nbCol,&stkAdr);
      if (nbRow*nbCol != 1)
        {
          Scierror(999, _("%s: Wrong size for input argument #%d: A real expected.\n"), fname, 1);
          return FALSE;
        }
      figNum = *istk(stkAdr);
      
      if (figNum < -1)
        {
          Scierror(999, _("%s: Wrong value for input argument #%d: Must be > %d expected.\n"), fname, 1, -1);
          return FALSE;
        }

      if (figNum != -1) /* Check that the figure exists */
        {
          if (getFigureFromIndex(figNum)==NULL)
            {
              Scierror(999, _("%s: Wrong value for input argument #%d: 'Graphic Window Number %d' does not exist.\n"), fname, 1, figNum);
              return FALSE;
            }
        }
    }
  else if ( GetType(1) == sci_handles )
    {
      GetRhsVar(1,GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);

          if (nbRow*nbCol != 1)
            {
              Scierror(999,_("%s: Wrong size for input argument #%d: A graphic handle expected.\n"),fname, 1);
              return FALSE;
            }
          pObj=sciGetPointerFromHandle((long)*hstk(stkAdr));
          
          if (pObj == NULL)
            {
              Scierror(999, _("%s: Wrong value for input argument #%d: this handle does not exist.\n"), fname, 1);
              return FALSE;
            }
          
          if ( (sciGetEntityType (pObj) != SCI_FIGURE) )
            {
              Scierror(999, _("%s: Wrong type for input argument #%d: A real or a Figure handle expected.\n"), fname, 1);
              return FALSE;
            }

          figNum = pFIGURE_FEATURE(pObj)->number;
    }
  else
    {
      Scierror(999, _("%s: Wrong type for input argument #%d: A real or Figure handle expected.\n"), fname, 1);
      return FALSE;
    }

  if (Rhs==2) /* New status */
    {
      if ((GetType(2) == sci_strings))
        {
          GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&nbRow,&nbCol,&param);
          if (nbRow*nbCol != 1)
            {
              Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 2);
              return FALSE;
            }
          
          if ( (strcmp(param[0],"off")==0) || (strcmp(param[0],"on")==0) )
            {
              setToolbarVisible(figNum, strcmp(param[0],"on")==0);
            }
          else
            {
              Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 2, "on", "off");
              return FALSE;
            }
        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
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
  
  nbCol = 1;
  nbRow = (int)strlen(Output);
  CreateVarFromPtr(Rhs+ 1,STRING_DATATYPE,&nbRow,&nbCol,&Output);
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
