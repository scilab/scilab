/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
 *  Copyright (C) 2011 - DIGITEO - Bruno JOFRET
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

#ifndef __LOCALIZATION_GW_HXX__
#define __LOCALIZATION_GW_HXX__

#include "cpp_gateway_prototype.hxx"

extern "C"
{
#include "dynlib_localization_gw.h"
}

class LocalizationModule
{
private :
    LocalizationModule() {};
    ~LocalizationModule() {};

public :
    LOCALIZATION_GW_IMPEXP static int Load();
    LOCALIZATION_GW_IMPEXP static int Unload()
    {
        return 1;
    }
};
CPP_GATEWAY_PROTOTYPE(sci_setdefaultlanguage);
CPP_GATEWAY_PROTOTYPE(sci_getdefaultlanguage);
CPP_GATEWAY_PROTOTYPE(sci_setlanguage);
CPP_GATEWAY_PROTOTYPE(sci_getlanguage);

#endif /* !__LOCALIZATION_GW_HXX__ */
