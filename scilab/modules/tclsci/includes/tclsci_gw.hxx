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

#ifndef __TCLSCI_GW_HXX__
#define __TCLSCI_GW_HXX__

#include <string>

extern "C"
{
#include "dynlib_tclsci.h"
}

class TclsciModule
{
private :
    TclsciModule() {};
    ~TclsciModule() {};
    static int LoadDeps(const std::wstring& _functionName);

public :
    TCLSCI_IMPEXP static int Load();
    TCLSCI_IMPEXP static int Unload()
    {
        return 1;
    }
};

#endif /* __TCLSCI_GW_HXX__ */
