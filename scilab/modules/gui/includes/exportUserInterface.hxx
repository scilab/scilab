/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Functions to export files
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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

