/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
/* file: StringBox.c                                                      */
/* desc : Contains a set of functions to compute the bounding box of a    */
/*        text                                                            */
/*------------------------------------------------------------------------*/

#include "StringBox.h"
#include "GetProperty.h"
#include "axesScale.h"
#include "math_graphics.h"
#include "MALLOC.h"
#include "CurrentObjectsManagement.h"
#include "ObjectSelection.h"
#include "BuildObjects.h"
#include "DestroyObjects.h"
#include "GraphicSynchronizerInterface.h"
#include "SetProperty.h"

/*-------------------------------------------------------------------------------*/
void getTextBoundingBox(char ** text, int nbRow, int nbCol,
                        double xPos, double yPos,
                        double angle, int fontId, double fontSize,
                        double corners[4][2])
{
 
  /* first step, create at text object */
  sciPointObj * parentFigure = sciGetCurrentFigure();
  sciPointObj * parentSubwin = sciGetCurrentSubWin();
  int defaultColor = 0; /* color does not matter */
  sciPointObj * pText = NULL;

  /* Update subwin scale if needed */
  updateSubwinScale(parentSubwin);

  startFigureDataWriting(parentFigure);
  
  pText = allocateText(parentSubwin,
                       text, nbRow, nbCol,
                       xPos, yPos,
                       TRUE,
                       NULL,
                       FALSE,
                       &defaultColor, &defaultColor,
                       FALSE, FALSE, FALSE,
                       ALIGN_LEFT);

  /* make it invisible to be sure */
  sciSetVisibility(pText, FALSE);

  sciSetFontOrientation(pText, DEG2RAD(angle));

  sciSetFontSize(pText, fontSize);
  sciSetFontStyle(pText, fontId);

  /* Then get its bounding box */
  /* update stringbox */
  updateTextBounds(pText);

  /* get the string box */
  sciGet2dViewBoundingBox( pText, corners[0], corners[1], corners[2], corners[3]) ;

  /* Finally destroy it */
  deallocateText(pText);

  endFigureDataWriting(parentFigure);

}
/*-------------------------------------------------------------------------------*/

