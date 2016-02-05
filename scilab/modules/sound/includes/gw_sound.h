/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
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

/*--------------------------------------------------------------------------*/
#ifndef __GW_SOUND__
#define __GW_SOUND__
/*--------------------------------------------------------------------------*/
#include "dynlib_sound.h"
/*--------------------------------------------------------------------------*/
SOUND_IMPEXP int gw_sound(void);
/*--------------------------------------------------------------------------*/
SOUND_IMPEXP int sci_beep(char *fname, void* pvApiCtx);
SOUND_IMPEXP int sci_PlaySound(char *fname, void* pvApiCtx);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_SOUND__ */
/*--------------------------------------------------------------------------*/
