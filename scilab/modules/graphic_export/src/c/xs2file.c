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

#include <string.h>

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
BOOL isVectorialExport(ExportFileType fileType);
/*--------------------------------------------------------------------------*/
int xs2file(char * fname, ExportFileType fileType )
{
	CheckLhs(0,1);
	if (isVectorialExport(fileType))
	{
		CheckRhs(2,3);
	}
	else
	{
		CheckRhs(2,2);
	}

	if ( (GetType(2) == sci_strings) && IsAScalar(1) )
	{
		int m1,n1,l1;
		int figurenum = -1;
		char * fileName = NULL;
		int status;
		ExportOrientation orientation = EXPORT_PORTRAIT; /* default orientation */

		/* get figure number */
		GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
		figurenum = *istk(l1);

		/* Check that figure number exists */
		if (!sciIsExistingFigure(figurenum))
		{
			Scierror(999, "%s: Figure with figure_id %d does not exist.\n",fname, figurenum);
			LhsVar(1)=0;
			return -1;
		}

		/* get file name */
		GetRhsVar(2,STRING_DATATYPE,&m1,&n1,&l1);
		fileName = cstk(l1);

		/* Get orientation */
		if (Rhs == 3)
		{
			int nbCol;
			int nbRow;
			int stackPointer;
			char * sciOrientation;
			/* vectorial export with orientation specified */
			if (GetType(3) != sci_strings)
			{
				Scierror(999,_("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"),fname, 3, "portrait", "landscape");
				return 0;
			}
			GetRhsVar(3,STRING_DATATYPE,&nbRow,&nbCol,&stackPointer);
			sciOrientation = cstk(stackPointer);
			/* Value should be 'landscape' or 'portrait' but check only the first character */
			/* for compatibility with Scilab 4*/
			if (strcmp(sciOrientation, "landscape") == 0 || strcmp(sciOrientation, "l") == 0)
			{
				orientation = EXPORT_LANDSCAPE;
			}
			else if(strcmp(sciOrientation, "portrait") == 0 || strcmp(sciOrientation, "p") == 0)
			{
				orientation = EXPORT_PORTRAIT;
			}
			else
			{
				Scierror(999,_("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"),fname, 3, "portrait", "landscape");
				return 0;
			}
		}

		/* Call the function for exporting file */
		status = exportToFile(getFigureFromIndex(figurenum), fileName, fileType, orientation);

		/* treat errors */
		switch(status)
		{
		case EXPORT_UNKNOWN_GLEXCEPTION_ERROR :
			Scierror(999,_("%s: OpenGL error during export.\n"),fname);
			break;
		case EXPORT_IOEXCEPTION_ERROR :
			Scierror(999,_("%s: Unable to create export file, permission denied.\n"),fname);
			break;
		case EXPORT_INVALID_FILE :
			Scierror(999,_("%s: Unable to create export file, permission denied.\n"),fname);
			break;
		case EXPORT_GL2PS_ERROR :
			Scierror(999,_("%s: GL2PS error during export.\n"),fname);
			break;
		case EXPORT_GL2PS_OVERFLOW :
			Scierror(999,_("%s: Unable to create export file, figure is too complex.\n"),fname);
			break;
		case EXPORT_GL2PS_UNINITIALIZED :
			Scierror(999,_("%s: GL2PS error during export.\n"),fname);
			break;
		default :
			break;
		}

	}
	else
	{
		if ( !IsAScalar(1) )
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: An Integer expected.\n"),fname, 1);
			return 0;
		}
		if ( GetType(2) != sci_strings)
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: Single character string expected.\n"),fname, 2);
			return 0;
		}
	}

	LhsVar(1)=0;
	return 0;
}
/*--------------------------------------------------------------------------*/
BOOL isVectorialExport(ExportFileType fileType)
{
	return   fileType == EPS_EXPORT
		  || fileType == PS_EXPORT
		  || fileType == PDF_EXPORT
		  || fileType == SVG_EXPORT;
}
/*--------------------------------------------------------------------------*/
