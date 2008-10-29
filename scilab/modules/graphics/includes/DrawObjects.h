/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001 - 2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002 - 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004 - 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------
 *    Graphic library 
 *    newGraph Library header
 *    Comment:
 *    This file contains all functions used to Draw the content of a window.
 *    The main functions is sciDrawObj that draws the objects recursively.
 --------------------------------------------------------------------------*/

#include "ObjectStructure.h"
#include "HandleManagement.h"


#ifndef __SCI_DRAWINGS__
#define __SCI_DRAWINGS__

#include "BOOL.h"
#include "PloEch.h"
#include "StringMatrix.h"
#include "Format.h"


/*******************************************************/

extern void sciRedrawFigure(void); /* DRAWINGS */
void sciClearFigure(sciPointObj * pFigure) ; /* DRAWINGS */
extern void sciXbasc(void);  /* DRAWINGS */
extern void sciXclear(void); /* DRAWINGS */


extern void sciGetDisplayedBounds( sciPointObj * pSubWin,
                                   double      * xmin   ,
                                   double      * xmax   ,
                                   double      * ymin   ,
                                   double      * ymax   ,
                                   double      * zmin   ,
                                   double      * zmax    ) ;
extern BOOL sci_update_frame_bounds_3d(sciPointObj *pobj);  /* DRAWINGS */
extern BOOL sci_update_frame_bounds_2d(sciPointObj *pobj);  /* DRAWINGS */

extern int ComputeNbSubTics(sciPointObj * pobj, int nbtics, char logflag, const double * grads, int nbsubtics_input); /* DRAWINGS */

extern void DrawAxesIfRequired(sciPointObj*); /* DRAWINGS */
extern void DrawAxes(sciPointObj*); /* DRAWINGS */

extern int sciDrawObjIfRequired (sciPointObj * pobj); /* DRAWINGS */


/* Other functions coming from Entities.c (old file) */


/**DJ.Abdemouche 2003**/
void sciDrawFigure( int numFigure ) ;



/**
 * Show the pixmap buffer of a figure
 */
void showPixmap(sciPointObj * pFigure);

/**
 * Clear the pixmap buffer of a figure
 */
void clearPixmap(sciPointObj * pFigure);

/**
 * @return true if the figure contains some objects
 *         which needs to be displayed in 3D.
 */
BOOL needsDisplay(sciPointObj * pFigure);

#endif /* __SCI_DRAWINGS__ */
