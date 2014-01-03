/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - Scilab Enterprises - Clément DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef _SCO_UTILS_
#define _SCO_UTILS_

#include "BOOL.h"

/**
 * Find the child with kind
 *
 * \param parent the parent graphic object to search
 * \param type the type to search
 * \param position the n child of kind (starting at 0)
 * \return the found child
 */
int findChildWithKindAt(int parent, int type, const int position);

/**
 * Set the labels of the axe
 *
 * \param iAxeUID the axe uid
 * \param _pstName the property name (eg. __GO_X_AXIS_LABEL__ or __GO_Y_AXIS_LABEL__)
 * \param label the label to set
 */
BOOL setLabel(int iAxeUID, int _iName, char* pstLabel);

#endif /* _SCO_UTILS_ */
