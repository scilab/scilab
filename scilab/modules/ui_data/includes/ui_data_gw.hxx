/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __UI_DATA_GW_HXX__
#define __UI_DATA_GW_HXX__

#include "cpp_gateway_prototype.hxx"
#include "c_gateway_prototype.h"
#include "dynlib_ui_data.h"

class UiDataModule
{
private:
    UiDataModule() {};
    ~UiDataModule() {};
public:
    UI_DATA_IMPEXP static int Load();
    UI_DATA_IMPEXP static int Unload()
    {
        return 1;
    }
};

CPP_GATEWAY_PROTOTYPE(sci_browsevar);
STACK_GATEWAY_PROTOTYPE(sci_editvar);
CPP_GATEWAY_PROTOTYPE(sci_filebrowser);
CPP_GATEWAY_PROTOTYPE(sci_updatebrowsevar);
CPP_GATEWAY_PROTOTYPE(sci_closeEditvar);

#endif /* __UI_DATA_GW_HXX__ */
