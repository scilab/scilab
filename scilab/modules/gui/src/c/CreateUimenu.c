/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Create Java object matching Scilab uimenu
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

#include "CreateUimenu.h"
#include "graphicObjectProperties.h"
#include "createGraphicObject.h"

/**
 * CreateUimenu
 * This function creates Uimenu structure.
 * @return created uimenu UID
 */
int CreateUimenu(void)
{
    return createGraphicObject(__GO_UIMENU__);
}
