/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Calixte DENIZET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

#ifndef __GW_PREFERENCES__
#define __GW_PREFERENCES__
/*--------------------------------------------------------------------------*/
#include "dynlib_preferences_scilab.h"
/*--------------------------------------------------------------------------*/
PREFERENCES_SCILAB_IMPEXP int gw_preferences(void);
int sci_addModulePreferences(char * fname, unsigned long l);
int sci_removeModulePreferences(char * fname, unsigned long l);
int sci_preferences(char * fname, unsigned long l);
/*--------------------------------------------------------------------------*/
#endif /* __GW_PREFERENCES__ */
/*--------------------------------------------------------------------------*/
