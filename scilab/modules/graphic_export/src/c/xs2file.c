/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * desc : interface for xs2file routine
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "xs2file.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "WindowList.h"
#include "IsAScalar.h"
#include "CurrentObjectsManagement.h"
#include "localization.h"
#include "SetJavaProperty.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int xs2file(char * fname, ExportFileType fileType )
{
  CheckLhs(0,1);
  CheckRhs(2,2);
  if ( (GetType(2) == sci_strings) && IsAScalar(1) )
  {
    integer m1,n1,l1;
    int figurenum=-1;
    GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
    figurenum = *istk(l1);
    if (figurenum>=0)
    {
      char * fileName=NULL;
	  int status;
      GetRhsVar(2,STRING_DATATYPE,&m1,&n1,&l1);
      fileName = cstk(l1);

      /* Call the function for exporting file */
      status = exportToFile(getFigureFromIndex(figurenum), fileName, fileType);

	  switch(status)
	  {
		case EXPORT_UNKNOWN_GLEXCEPTION_ERROR : Scierror(999,_("%s: OpenGL error during export.\n"),fname);break;
		case EXPORT_IOEXCEPTION_ERROR : Scierror(999,_("%s: Unable to create export file, permission denied.\n"),fname);break;
		case EXPORT_INVALID_FILE : Scierror(999,_("%s: Unable to create export file, permission denied.\n"),fname);break;
		case EXPORT_GL2PS_ERROR : Scierror(999,_("%s: GL2PS error during export.\n"),fname);break;
		case EXPORT_GL2PS_OVERFLOW : Scierror(999,_("%s: Unable to create export file, figure is too complex.\n"),fname);break;
		case EXPORT_GL2PS_UNINITIALIZED : Scierror(999,_("%s: GL2PS error during export.\n"),fname);break;
		default : return 0;
      }

    }
    else
    {
      Scierror(999,_("%s: Wrong input argument: %s expected.\n"),fname,">=0");
      return 0;
    }

  }
  else
  {
    if ( IsAScalar(1) )
    {
      Scierror(999,_("%s: Wrong type for first input argument: integer scalar expected.\n"),fname);
      return 0;
    }
    if ( GetType(2) != sci_strings)
    {
      Scierror(999,_("%s: Wrong type for second input argument: String expected.\n"),fname);
      return 0;
    }
  }

  LhsVar(1)=0;
  return 0;
}
/*--------------------------------------------------------------------------*/
