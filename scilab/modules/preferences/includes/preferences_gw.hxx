/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Cedric Delamarre
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __PREFERENCES_GW_HXX__
#define __PREFERENCES_GW_HXX__

#include <string>
#include "c_gateway_prototype.h"
#include "dynlib_preferences_scilab.h"

class PreferencesModule
{
private :
    PreferencesModule() {};
    ~PreferencesModule() {};
public :
    PREFERENCES_SCILAB_IMPEXP  static int LoadDeps(std::wstring _functionName);
    PREFERENCES_SCILAB_IMPEXP static int Load();
    PREFERENCES_SCILAB_IMPEXP static int Unload()
    {
        return 1;
    }
};

C_GATEWAY_PROTOTYPE(sci_addModulePreferences);
C_GATEWAY_PROTOTYPE(sci_removeModulePreferences);
C_GATEWAY_PROTOTYPE(sci_preferences);

#endif /* __PREFERENCES_GW_HXX__ */
