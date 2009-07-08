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

#ifndef _AFFICH_H_
#define _AFFICH_H_

#include "machine.h" /* C2F */

/**
* Update of one element of the text object used by Affich2 block.
*/
void C2F(settxtel)(int * numRow, 
				   int * numCol,
				   double * winNum,
				   double * textIndex,
				   char * value,
				   int valueLength);

/**
* Printing of the text object element used by Affich2 routine
*/
void C2F(affup2)( double * winNum, 
				 double * textIndex);
/**
* Printing of the text object element used by affich.f routine
*/

void C2F(affichup)( double * textIndex,
                    double * winNum,
                    char * value,
                    int valueLength);
#endif

