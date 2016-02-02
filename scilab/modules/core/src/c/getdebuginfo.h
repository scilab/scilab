/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRU
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
#ifndef __GETDEBUGINFO_H__
#define __GETDEBUGINFO_H__

/**
 * The commun stuff for the debug info retrieving
 */

#define NB_DEBUG_ELEMENT 255

typedef struct debugMessage
{
    char *description;
    char *value;
} debug_message;

#endif /* __GETDEBUGINFO_H__ */
/*--------------------------------------------------------------------------*/

