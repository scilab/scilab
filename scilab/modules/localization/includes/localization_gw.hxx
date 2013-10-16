/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
 *  Copyright (C) 2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
};
CPP_GATEWAY_PROTOTYPE(sci_setdefaultlanguage);
CPP_GATEWAY_PROTOTYPE(sci_getdefaultlanguage);
CPP_GATEWAY_PROTOTYPE(sci_setlanguage);
CPP_GATEWAY_PROTOTYPE(sci_getlanguage);

#endif /* !__LOCALIZATION_GW_HXX__ */
