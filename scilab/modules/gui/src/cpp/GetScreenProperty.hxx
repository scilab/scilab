/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Get the position of an uicontrol
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

#ifndef __GETSCREENPROPERTY_HXX__
#define __GETSCREENPROPERTY_HXX__

#include "CallScilabBridge.hxx"
#include "UnitsConversion.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"
#include "os_string.h"
#include "returnProperty.h"
#include "GetScreenProperty.h"
#include "localization.h"
#include "Scierror.h"
}

#endif /* !__GETSCREENPROPERTY_HXX__ */
