/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
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

#ifndef __HELPTOOLS_GW_HXX__
#define __HELPTOOLS_GW_HXX__

#include "function.hxx"
#include "context.hxx"

extern "C"
{
#include "dynlib_helptools.h"
}

class HelptoolsModule
{
private :
    HelptoolsModule() {};
    ~HelptoolsModule() {};

    static bool loadedDep;
    static void LoadDeps(void);

public :
    HELPTOOLS_IMPEXP static int Load();
    HELPTOOLS_IMPEXP static int Unload()
    {
        return 1;
    }
};

#endif /* !__HELPTOOLS_GW_HXX__ */
