/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001 - 2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002 - 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004 - 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - 2011 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
GRAPHIC_OBJECTS_IMPEXP long getHandle(char const* UID);

/**
 * @brief Get the uid of an handle.
 *
 * @param a valid handle
 * @return a reference to an existing uid or null if invalid.
 */
GRAPHIC_OBJECTS_IMPEXP char const* getObjectFromHandle(long handle);

#endif /* !__HANDLE_MANAGEMENT_H__ */
