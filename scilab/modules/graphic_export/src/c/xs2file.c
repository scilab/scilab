/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Fabrice Leray
* Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* desc : interface for xs2file routine
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <string.h>
#include <stdio.h>

#include "xs2file.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "WindowList.h"
#include "IsAScalar.h"
#include "CurrentObjectsManagement.h"
#include "localization.h"
#include "SetJavaProperty.h"
#include "Scierror.h"
#include "expandPathVariable.h"
#include "PATH_MAX.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "HandleManagement.h"
#include "GraphicSynchronizerInterface.h"
#include "../../../graphics/src/c/getHandleProperty/getPropertyAssignedValue.h"
/*--------------------------------------------------------------------------*/
static BOOL isVectorialExport(ExportFileType fileType);
/*--------------------------------------------------------------------------*/
int xs2file(char * fname, ExportFileType fileType )
{
	/* Check input and output sizes */
	CheckLhs(0,1);
	if (isVectorialExport(fileType))
	{
		CheckRhs(2,3);
	}
	else
	{
		CheckRhs(2,2);
	}

	if (GetType(1) != sci_matrix && GetType(1) != sci_handles)
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: An integer or a handle expected.\n"),fname, 1);
	  LhsVar(1) = 0;
	  C2F(putlhsvar)();
		return 0;
	}

	if ( (GetType(2) == sci_strings) )
	{	
		char **fileName = NULL;
		char *real_filename = NULL;
		ExportOrientation orientation = EXPORT_PORTRAIT; /* default orientation */
		long int lout = 0;
		int out_n = 0;
		int m1 = 0, n1 = 0, l1 = 0;
		int figurenum = -1;
    sciPointObj* figurePtr = NULL;
		int status = 0;

		/* get handle by figure number */
    if(GetType(1) == sci_matrix)
    {
  		GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
      if(m1*n1 != 1)
      {
        Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname, 1);        
			  LhsVar(1) = 0;
			  C2F(putlhsvar)();
			  return 0;
      }

	  	figurenum = *istk(l1);
		  if (!sciIsExistingFigure(figurenum))
		  {
			  Scierror(999, "%s: Input argument #%d must be a valid figure_id.\n",fname, 1);
			  LhsVar(1) = 0;
			  C2F(putlhsvar)();
			  return 0;
		  }
      figurePtr = getFigureFromIndex(figurenum);
    }
    /* check given handle */
    if(GetType(1) == sci_handles)
    {
      GetRhsVar(1,GRAPHICAL_HANDLE_DATATYPE,&m1,&n1,&l1);
      if(m1*n1 != 1)
      {
        Scierror(999,_("%s: Wrong size for input argument #%d: A graphic handle expected.\n"),fname, 1);        
			  LhsVar(1) = 0;
			  C2F(putlhsvar)();
			  return 0;
      }
      figurePtr = sciGetPointerFromHandle(getHandleFromStack(l1));

      if(figurePtr == NULL)
      {
			  Scierror(999, "%s: Input argument #%d must be a valid handle.\n",fname, 1);
			  LhsVar(1) = 0;
			  C2F(putlhsvar)();
			  return 0;        
      }
      startFigureDataReading(figurePtr);
      if(sciGetEntityType(figurePtr)!=SCI_FIGURE)
      {
			  Scierror(999, "%s: Input argument #%d must be a handle on a figure.\n", fname, 1);
			  LhsVar(1) = 0;
			  C2F(putlhsvar)();
			  return 0;        
      }
      endFigureDataReading(figurePtr);
    }

		/* get file name */
		GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&fileName);
		if (m1*n1 == 1)
		{
			if (Rhs == 3)
			{
				int nbCol = 0;
				int nbRow = 0;
				char **sciOrientation = NULL;

				if (GetType(3) != sci_strings)
				{
					freeArrayOfString(fileName,m1*n1);
					Scierror(999,_("%s: Wrong type for input argument #%d: Single character string expected.\n"),fname, 3);
					return 0;
				}

				GetRhsVar(3,MATRIX_OF_STRING_DATATYPE,&nbRow,&nbCol,&sciOrientation);
				if (nbRow*nbCol == 1)
				{
					/* Value should be 'landscape' or 'portrait' but check only the first character */
					/* for compatibility with Scilab 4*/
					if (strcmp(sciOrientation[0], "landscape") == 0 || strcmp(sciOrientation[0], "l") == 0)
					{
						freeArrayOfString(sciOrientation,nbRow*nbCol);
						orientation = EXPORT_LANDSCAPE;
					}
					else if(strcmp(sciOrientation[0], "portrait") == 0 || strcmp(sciOrientation[0], "p") == 0)
					{
						freeArrayOfString(sciOrientation,nbRow*nbCol);
						orientation = EXPORT_PORTRAIT;
					}
					else
					{
						freeArrayOfString(fileName,m1*n1);
						freeArrayOfString(sciOrientation,nbRow*nbCol);
						Scierror(999,_("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"),fname, 3, "portrait", "landscape");
						return 0;
					}
				}
				else
				{
					freeArrayOfString(fileName,m1*n1);
					freeArrayOfString(sciOrientation,nbRow*nbCol);
					Scierror(999,_("%s: Wrong size for input argument #%d: Single character string expected.\n"),fname, 3);
					return 0;
				}
			}

			/* Replaces SCI, ~, HOME, TMPDIR by the real path */
			real_filename = expandPathVariable(fileName[0]);

			/* Call the function for exporting file */
			status = exportToFile(figurePtr, real_filename, fileType, orientation);

			/* free pointers no more used */
			if (real_filename){FREE(real_filename);real_filename = NULL;}
			freeArrayOfString(fileName,m1*n1);

			/* treat errors */
			switch(status)
			{
			case EXPORT_UNKNOWN_GLEXCEPTION_ERROR :
				Scierror(999,_("%s: OpenGL error during export.\n"),fname);
				return 0;
			case EXPORT_IOEXCEPTION_ERROR :
				Scierror(999,_("%s: Unable to create export file, permission denied.\n"),fname);
				return 0;
			case EXPORT_INVALID_FILE :
				Scierror(999,_("%s: Unable to create export file, permission denied.\n"),fname);
				return 0;
			case EXPORT_GL2PS_ERROR :
				Scierror(999,_("%s: GL2PS error during export.\n"),fname);
				return 0;
			case EXPORT_GL2PS_OVERFLOW :
				Scierror(999,_("%s: Unable to create export file, figure is too complex.\n"),fname);
				return 0;
			case EXPORT_GL2PS_UNINITIALIZED :
				Scierror(999,_("%s: GL2PS error during export.\n"),fname);
				return 0;
			default :
				// NO ERROR
				break;
			}
		}
		else
		{
			freeArrayOfString(fileName,m1*n1);
			Scierror(999,_("%s: Wrong size for input argument #%d: Single character string expected.\n"),fname, 2);
			return 0;
		}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: Single character string expected.\n"),fname, 2);
		return 0;
	}

	LhsVar(1) = 0;
	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
static BOOL isVectorialExport(ExportFileType fileType)
{
	return   fileType == EPS_EXPORT
		|| fileType == PS_EXPORT
		|| fileType == PDF_EXPORT
		|| fileType == SVG_EXPORT;
}
/*--------------------------------------------------------------------------*/

