/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/ 
#ifndef __GW_SOUND__
#define __GW_SOUND__
/*--------------------------------------------------------------------------*/ 
#include "dynlib_sound.h"
#include "api_common.h"
/*--------------------------------------------------------------------------*/ 
SOUND_IMPEXP int gw_sound(void);
/*--------------------------------------------------------------------------*/ 
SOUND_IMPEXP int sci_Beep(char *fname,unsigned long fname_len);
SOUND_IMPEXP int sci_Playsound(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/ 
#endif /*  __GW_SOUND__ */
/*--------------------------------------------------------------------------*/
