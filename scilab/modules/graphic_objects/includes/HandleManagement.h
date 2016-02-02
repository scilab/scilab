/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001 - 2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002 - 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004 - 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - 2011 - DIGITEO - Bruno JOFRET
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

/*------------------------------------------------------------------------
 *    Graphic library
 *    newGraph Library header
 *    Comment:
 *    This file contains all functions used in graphics handle management.
 --------------------------------------------------------------------------*/

#ifndef __HANDLE_MANAGEMENT_H__
#define __HANDLE_MANAGEMENT_H__

#include "dynlib_graphic_objects.h"

/**
 * @brief Get the handle for an UID.
 *
 * This should also allocate a new handle on demand. UID can be safely
 * deallocated after this call.
 *
 * @param any uid
 * @return a handle mapped to the uid
 */
GRAPHIC_OBJECTS_IMPEXP long getHandle(int iUID);

/**
 * @brief Get the uid of an handle.
 *
 * @param a valid handle
 * @return a reference to an existing uid or null if invalid.
 */
GRAPHIC_OBJECTS_IMPEXP int getObjectFromHandle(long handle);

#endif /* !__HANDLE_MANAGEMENT_H__ */
