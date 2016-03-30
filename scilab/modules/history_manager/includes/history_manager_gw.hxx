/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
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

#ifndef __HISTORY_MANAGER_GW_HXX__
#define __HISTORY_MANAGER_GW_HXX__

#include "cpp_gateway_prototype.hxx"

extern "C"
{
#include "dynlib_history_manager_gw.h"
}
class HistoryManagerModule
{
private :
    HistoryManagerModule() {};
    ~HistoryManagerModule() {};
public :
    HISTORY_MANAGER_GW_IMPEXP static int Load();
    HISTORY_MANAGER_GW_IMPEXP static int Unload()
    {
        return 1;
    }
};

CPP_GATEWAY_PROTOTYPE(sci_loadhistory);
CPP_GATEWAY_PROTOTYPE(sci_addhistory);
CPP_GATEWAY_PROTOTYPE(sci_displayhistory);
CPP_GATEWAY_PROTOTYPE(sci_gethistoryfile);
CPP_GATEWAY_PROTOTYPE(sci_gethistory);
CPP_GATEWAY_PROTOTYPE(sci_historymanager);
CPP_GATEWAY_PROTOTYPE(sci_historysize);
CPP_GATEWAY_PROTOTYPE(sci_removelinehistory);
CPP_GATEWAY_PROTOTYPE(sci_resethistory);
CPP_GATEWAY_PROTOTYPE(sci_saveafterncommands);
CPP_GATEWAY_PROTOTYPE(sci_saveconsecutivecommands);
CPP_GATEWAY_PROTOTYPE(sci_savehistory);
CPP_GATEWAY_PROTOTYPE(sci_sethistoryfile);

#endif /* !__HISTORY_MANAGER_GW_HXX__ */
