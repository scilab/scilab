/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2008 - INRIA - Simone Mannori
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/ 
#include <stdio.h>
#include "GetProperty.h"
#include "SetProperty.h"
#include "ObjectSelection.h"
#include "HandleManagement.h"
#include "CurrentObjectsManagement.h"
#include "DrawingBridge.h"
#include "StringMatrix.h"
#include "affich.h"
#include "WindowList.h"
/*--------------------------------------------------------------------------*/ 
/** Index of the text object within the block (ie compound) */
#define TEXT_INDEX_IN_BLOCK 1

/*---------------------------------------------------------------------------------*/
/**
 * Retrieve the text object the affich routine use.
 * @param winNum index of the figure in which the text lies in.
 * @param compoundIndex index within the children of the current subwin of the
 *                      parent compound of the text.
 */
static sciPointObj * getAffichTextObject(int winNum, int compoundIndex);
/*---------------------------------------------------------------------------------*/
static sciPointObj * getAffichTextObject(int winNum, int compoundIndex)
{
  sciPointObj * parentFigure = getFigureFromIndex(winNum);
  sciPointObj * parentSubwin = sciGetFirstTypedSelectedSon(parentFigure, SCI_SUBWIN);
  sciPointObj * parentCompound = sciGetIndexedSon(parentSubwin, compoundIndex);

  return sciGetIndexedSon(parentCompound, TEXT_INDEX_IN_BLOCK);
}



/*---------------------------------------------------------------------------------*/
void C2F(settxtel)( int * numRow, 
                    int * numCol,
                    double * winNum,
                    double * textIndex,
                    char * value,
                    int valueLength)
{
  int nbRow,nbCol;
  /* First step, retreive text handle */
  /* For now we got the index of the block coupound within the axes children */
  /* Get the text handle */
  /* NB it should be better to test the correctness of the handle only once! */
  sciPointObj * pText = getAffichTextObject((int) *winNum, ((int) *textIndex) - 1);
  if (pText==(sciPointObj *)NULL) return;
  if (sciGetEntityType (pText)== SCI_TEXT) {
    sciGetTextSize(pText, &nbRow, &nbCol );
    if (nbRow>=*numRow && nbCol>=*numCol) {
      StringMatrix * mat = sciGetText(pText);
      /* Set matrix element */
      copyStrMatElement(mat, (*numRow) - 1, (*numCol) - 1, value);
    }
  }
}
/*---------------------------------------------------------------------------------*/
void C2F(affup2)( double * winNum, 
                  double * textIndex)
{
  /* First step, retreive text handle */
  /* For now we got the index of the block compound within the axes children */
  /* Get the text handle */
  sciPointObj * pText = getAffichTextObject((int) *winNum, ((int) *textIndex) - 1);

  /* StringMatrix * mat = sciGetText(pText); */
  if (pText==(sciPointObj *)NULL) return;
  /* Second step draw the object */
  sciDrawObj(pText);
}
/*---------------------------------------------------------------------------------*/

void C2F(affichup)( double * textIndex,
                    double * winNum,
                    char * value,
                    int valueLength)
{
 int nbRow,nbCol;
  /* First step, retreive text handle */
  /* For now we got the index of the block coupound within the axes children */
  sciPointObj * pText = getAffichTextObject((int) *winNum, ((int) *textIndex) - 1);
 if (pText==(sciPointObj *)NULL) return;
  if (sciGetEntityType (pText)== SCI_TEXT) {
  /* Second step update text data */
    sciSetText(pText, &value, 1, 1);
    sciDrawObj(pText);
  }
}
/*---------------------------------------------------------------------------------*/

