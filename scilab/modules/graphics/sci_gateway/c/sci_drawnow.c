/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_drawnow.c                                                    */
/* desc : interface for drawnow routine                                   */
/*------------------------------------------------------------------------*/

#include "sci_drawnow.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "GetProperty.h"
#include "ObjectStructure.h"
#include "DrawObjects.h"
#include "CurrentObjectsManagement.h"
#include "DrawingBridge.h"
#include "GraphicSynchronizerInterface.h"
#include "SetProperty.h"

/*--------------------------------------------------------------------------*/
int sci_drawnow(char *fname,unsigned long fname_len)
{ 
	sciPointObj *pfigure = NULL;
	CheckRhs(0,0);
	CheckLhs(0,1); 

	if (Rhs <= 0) 
	{
		startGraphicDataWriting();
		pfigure = sciGetCurrentFigure() ;
		sciSetImmediateDrawingMode(pfigure, TRUE);
		endGraphicDataWriting();

		sciDrawObj(pfigure);
	}

	LhsVar(1) = 0;
	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
