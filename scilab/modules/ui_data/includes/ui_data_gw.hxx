/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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
