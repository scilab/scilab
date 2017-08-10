/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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

/*------------------------------------------------------------------------*/
/* file: CheckTicksProperty.h                                             */
/* desc : generic function to retrieve the assigned x_ticks, y_ticks or   */
/*        z_ticks property                                                */
/*------------------------------------------------------------------------*/

#ifndef _CHECK_TICKS_PROPERTY_C_
#define _CHECK_TICKS_PROPERTY_C_

#include "getPropertyAssignedValue.h"

/**
 * create a new assigne dlist which allow to easily retrieve the ticks
 * properties from the stack. Moreover the functgion checked if the list
 * contains the right properties.
 * @return the newly created list or NULL if an error occurred.
 */
AssignedList * createTlistForTicks(void* _pvCtx);

#endif /* _CHECK_TICKS_PROPERTY_C_ */
