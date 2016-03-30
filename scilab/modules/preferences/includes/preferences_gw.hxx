/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Cedric Delamarre
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
    PREFERENCES_SCILAB_IMPEXP  static int LoadDeps(const std::wstring& _functionName);
    PREFERENCES_SCILAB_IMPEXP static int Load();
    PREFERENCES_SCILAB_IMPEXP static int Unload()
    {
        return 1;
    }
};

STACK_GATEWAY_PROTOTYPE(sci_addModulePreferences);
STACK_GATEWAY_PROTOTYPE(sci_removeModulePreferences);
STACK_GATEWAY_PROTOTYPE(sci_preferences);

#endif /* __PREFERENCES_GW_HXX__ */
