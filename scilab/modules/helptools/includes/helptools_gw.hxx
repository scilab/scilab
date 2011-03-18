/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
    HELPTOOLS_IMPEXP static bool Load();
};

#endif /* !__HELPTOOLS_GW_HXX__ */
