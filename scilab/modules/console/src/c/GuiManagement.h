/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GUIMANAGEMENT_H__
#define __GUIMANAGEMENT_H__

/**
  * Sets Scilab parameters used to display data according the size of the console
  * @param nbRows number of rows that can be used to display data
  * @param nbCols number of columns that can be used to display data
  */
void setScilabLines(int nbRows, int nbCols);

/**
  * Sets Scilab parameters used to display data
  * @param nbRows number of rows that can be used to display data
  * @param nbCols number of columns that can be used to display data
  */
void forceScilabLines(int nbRows, int nbCols);

#endif /* __GUIMANAGEMENT_H__ */
/*--------------------------------------------------------------------------*/

