/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - DIGITEO - Sylvestre LEDRU
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2011 - DIGITEO - Antoine ELIAS
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
#ifndef __INITMACOSXENV_H__
#define __INITMACOSXENV_H__

#include "InitScilab.h"
/*
 * Specific wrapper for mac os X which is going to call realmin in a specific
 * thread.
 * Takes the same args as StartScilabEngine
 * @param argc
 * @param argv initial_script the initial script given to Scilab
 * @param iFileIndex initial script index in argv : the initial script given to Scilab
 * @return 1 if OK or 0 if KO
 */

int initMacOSXEnv(ScilabEngineInfo*);

#endif /* !__INITMACOSXENV_H__ */
