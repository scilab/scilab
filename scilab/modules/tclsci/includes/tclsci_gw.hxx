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
