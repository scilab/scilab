/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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

