/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRu
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

#ifndef __WITH_UMFPACK_H__
#define __WITH_UMFPACK_H__

#include "machine.h" /* C2F */
#include "dynlib_umfpack.h"
#include "BOOL.h"

/**
* check if it is with UMFPACK
* @param rep 1 (WITH) or 0 (WITHOUT)
* @return 0
*/
UMFPACK_IMPEXP int C2F(withumfpack)(int *rep);

/**
* check if it is with UMFPACK
* @return TRUE or FALSE
*/
UMFPACK_IMPEXP BOOL withumfpack(void);

#endif /* __WITH_UMFPACK_H__ */
/*--------------------------------------------------------------------------*/
