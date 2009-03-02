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
#include "cluni0.h"
#include "PATH_MAX.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
BOOL isVectorialExport(ExportFileType fileType);
/*--------------------------------------------------------------------------*/
int xs2file(char * fname, ExportFileType fileType )
{
	sciPointObj * exportedFigure = NULL;
	int nbRow = 0, nbCol = 0;
	int stackPointer = NULL;
	char * fileName = NULL;
	char *real_filename = NULL;
	ExportOrientation orientation = EXPORT_PORTRAIT; /* default orientation */
	int status;
	long int lout;
	int out_n;
	
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

	/* Get exported figure */
	if (IsAScalar(1))
	{
		/* window is specified by its number */
		int figureIndex;
		GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&nbRow,&nbCol,&stackPointer);
		figureIndex = *istk(stackPointer);
		if (!sciIsExistingFigure(figureIndex))
		{
			Scierror(999, "%s: Figure with figure_id %d does not exist.\n",fname, figureIndex);
			LhsVar(1)=0;
			return -1;
		}
		exportedFigure = getFigureFromIndex(figureIndex);
	}
	else if (GetType(1) == sci_handles)
	{
		/* handle given */
		GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stackPointer);
		if (nbRow * nbCol != 1)
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: An Integer or a 'Figure' handle expected.\n"),fname, 1);
			LhsVar(1)=0;
			return 0;
		}

		exportedFigure = sciGetPointerFromHandle((long) *hstk(stackPointer));

		if (exportedFigure == NULL)
		{
			Scierror(999,_("%s: The handle is not or no more valid.\n"),fname);
      LhsVar(1) = 0;
      return -1;
		}

		if (sciGetEntityType(exportedFigure) != SCI_FIGURE)
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: An Integer or a 'Figure' handle expected.\n"),fname, 1);
			LhsVar(1)=0;
			return 0;
		}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: An Integer or a 'Figure' handle expected.\n"),fname, 1);
		LhsVar(1)=0;
		return 0;
	}

	/* Get file name */
	if (GetType(2) == sci_strings)
	{
		GetRhsVar(2,STRING_DATATYPE,&nbCol,&nbRow,&stackPointer);
		fileName = cstk(stackPointer);
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: Single character string expected.\n"),fname, 2);
		LhsVar(1)=0;
		return 0;
	}

	/* Get orientation if specified */
	if (Rhs == 3)
	{
		char * sciOrientation;
		/* vectorial export with orientation specified */
		if (GetType(3) != sci_strings)
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: Single character string expected.\n"),fname, 3);
			LhsVar(1)=0;
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
			LhsVar(1)=0;
			return 0;
		}
	}
	/* Replaces SCI, ~, HOME, TMPDIR by the real path */
	lout = PATH_MAX + FILENAME_MAX;
	real_filename = (char*)MALLOC(sizeof(char)*lout);
				
	/* Replaces SCI, ~, HOME, TMPDIR by the real path */ 
	C2F(cluni0)(fileName, real_filename, &out_n, (long)strlen(fileName), lout);

	/* Call the function for exporting file */
	status = exportToFile(exportedFigure, real_filename, fileType, orientation);
	if (real_filename){FREE(real_filename);real_filename = NULL;}

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

