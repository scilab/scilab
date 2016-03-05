/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - Scilab Enterprises - Clément DAVID
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
