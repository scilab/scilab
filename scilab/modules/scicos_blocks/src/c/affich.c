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

/*---------"affich" Initialization ------------------------------------------------*/
void C2F(affichin)( int * fontId, 
                    int * fontSize,
                    int * color,
                    double * winNum, 
                    double * posX, 
                    double * posY,
                    double * width,
                    double * height,
                    double * textIndex)
{
  char * defaultText = "0.0";

  /* First step, retreive text handle */
  /* For now we got the index of the block coupound within the axes children */

  /* Get the text handle */
  sciPointObj * pText = getAffichTextObject((int) *winNum, ((int) *textIndex) - 1);

  /* Second step set text parameters */
  sciSetFontStyle(pText, *fontId);
  sciSetFontSize(pText, *fontSize);
  sciSetForeground(pText, -1);
  //** little right, half height
  sciSetTextPos(pText, *posX+(double) (0.10 * *width), *posY+(double) (0.4 * *height), 0.0); //** x, y, z :)
  sciSetUserSize(pText, *width, *height);
  sciSetAutoSize(pText, TRUE);
  sciSetText(pText, &defaultText, 1, 1);


  sciDrawObj(pText);



}
/*---------------------------------------------------------------------------------*/
void C2F(affichup)( double * textIndex,
                    double * winNum,
                    char * value,
                    int valueLength)
{
  /* First step, retreive text handle */
  /* For now we got the index of the block coupound within the axes children */
  sciPointObj * pText = getAffichTextObject((int) *winNum, ((int) *textIndex) - 1);

  /* Second step update text data */
  sciSetText(pText, &value, 1, 1);
  sciDrawObj(pText);



}
/*---------"affich2" Initialization ------------------------------------------------*/
void C2F(affin2)( int * fontId, 
                    int * fontSize,
                    int * color,
                    int * nbRow,
                    int * nbCol,
                    double * winNum, 
                    double * posX, 
                    double * posY,
                    double * width,
                    double * height,
                    double * textIndex)
{
  StringMatrix * mat = newEmptyStringMatrix(*nbRow, *nbCol);

  /* First step, retreive text handle */
  /* For now we got the index of the block coupound within the axes children */
  sciPointObj * pText = getAffichTextObject((int) *winNum, ((int) *textIndex) - 1);

  /* Second step set text parameters */
  sciSetFontStyle(pText, *fontId);
  sciSetFontSize(pText, *fontSize);
  sciSetForeground(pText, -1);
  sciSetTextPos(pText, *posX+(double) (0.10 * *width), *posY+(double) (0.2 * *height), 0.0); //** x, y, z :)
  sciSetUserSize(pText, *width, *height);
  sciSetAutoSize(pText, TRUE);
  sciSetStrings(pText, mat);

  sciDrawObj(pText);

  deleteMatrix(mat);


}
/*---------------------------------------------------------------------------------*/
void C2F(settxtel)( int * numRow, 
                    int * numCol,
                    double * winNum,
                    double * textIndex,
                    char * value,
                    int valueLength)
{

  /* First step, retreive text handle */
  /* For now we got the index of the block coupound within the axes children */
  /* Get the text handle */
  sciPointObj * pText = getAffichTextObject((int) *winNum, ((int) *textIndex) - 1);
  StringMatrix * mat = sciGetText(pText);

  /* Set matrix element */
  copyStrMatElement(mat, (*numRow) - 1, (*numCol) - 1, value);

}
/*---------------------------------------------------------------------------------*/
void C2F(affup2)( double * winNum, 
                  double * textIndex)
{
  /* First step, retreive text handle */
  /* For now we got the index of the block coupound within the axes children */
  /* Get the text handle */
  sciPointObj * pText = getAffichTextObject((int) *winNum, ((int) *textIndex) - 1);
  StringMatrix * mat = sciGetText(pText);

  /* Second step draw the object */
  sciDrawObj(pText);


}
/*---------------------------------------------------------------------------------*/
