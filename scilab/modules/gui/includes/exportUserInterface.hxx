/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Functions to export files
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

#ifndef _EXPORT_USER_INTERFACE_HXX_
#define _EXPORT_USER_INTERFACE_HXX_

#ifdef __cplusplus
extern "C" {
#endif

#include "dynlib_gui.h"
/**
 * Call the graphic export GUI
 * @param figreId id of the figure to export
 */
GUI_IMPEXP int exportUserInterface(int figureId);

#ifdef __cplusplus
}
#endif

#endif /* _EXPORT_USER_INTERFACE_HXX_ */

