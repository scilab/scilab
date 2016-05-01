/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
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

#ifndef __CORE_GW_HXX__
#define __CORE_GW_HXX__

#include "cpp_gateway_prototype.hxx"
#include "dynlib_core_gw.h"

class CoreModule
{
private :
    CoreModule() {};
    ~CoreModule() {};
public :
    CORE_GW_IMPEXP static int Load();
    CORE_GW_IMPEXP static int Unload()
    {
        return 1;
    }
};

CPP_GATEWAY_PROTOTYPE(sci_getmodules);
CPP_GATEWAY_PROTOTYPE(sci_clear);
CPP_GATEWAY_PROTOTYPE(sci_banner);
CPP_GATEWAY_PROTOTYPE(sci_quit);
CPP_GATEWAY_PROTOTYPE(sci_exit);
CPP_GATEWAY_PROTOTYPE(sci_exists);
CPP_GATEWAY_PROTOTYPE(sci_isdef);
CPP_GATEWAY_PROTOTYPE(sci_argn);
CPP_GATEWAY_PROTOTYPE(sci_global);
CPP_GATEWAY_PROTOTYPE(sci_clearglobal);
CPP_GATEWAY_PROTOTYPE(sci_isglobal);
CPP_GATEWAY_PROTOTYPE(sci_getscilabmode);
CPP_GATEWAY_PROTOTYPE(sci_getos);
CPP_GATEWAY_PROTOTYPE(sci_with_module);
CPP_GATEWAY_PROTOTYPE(sci_lasterror);
CPP_GATEWAY_PROTOTYPE(sci_getversion);
CPP_GATEWAY_PROTOTYPE(sci_warning);
CPP_GATEWAY_PROTOTYPE(sci_getmd5);
CPP_GATEWAY_PROTOTYPE(sci_error);
CPP_GATEWAY_PROTOTYPE(sci_pause);
CPP_GATEWAY_PROTOTYPE(sci_abort);
CPP_GATEWAY_PROTOTYPE(sci_who);
CPP_GATEWAY_PROTOTYPE(sci_sciargs);
CPP_GATEWAY_PROTOTYPE(sci_format);
CPP_GATEWAY_PROTOTYPE(sci_ieee);
CPP_GATEWAY_PROTOTYPE(sci_funcprot);
CPP_GATEWAY_PROTOTYPE(sci_mode);
CPP_GATEWAY_PROTOTYPE(sci_typename);
CPP_GATEWAY_PROTOTYPE(sci_what);
CPP_GATEWAY_PROTOTYPE(sci_where);
CPP_GATEWAY_PROTOTYPE(sci_getmemory);
CPP_GATEWAY_PROTOTYPE(sci_errclear);
CPP_GATEWAY_PROTOTYPE(sci_intppty);
CPP_GATEWAY_PROTOTYPE(sci_newfun);
CPP_GATEWAY_PROTOTYPE(sci_clearfun);
CPP_GATEWAY_PROTOTYPE(sci_funptr);
CPP_GATEWAY_PROTOTYPE(sci_analyzeroptions);
CPP_GATEWAY_PROTOTYPE(sci_macr2tree);
CPP_GATEWAY_PROTOTYPE(sci_predef);
CPP_GATEWAY_PROTOTYPE(sci_debug);
CPP_GATEWAY_PROTOTYPE(sci_recursionlimit);
CPP_GATEWAY_PROTOTYPE(sci_type);
CPP_GATEWAY_PROTOTYPE(sci_list_gw);
CPP_GATEWAY_PROTOTYPE(sci_struct_gw);
CPP_GATEWAY_PROTOTYPE(sci_null);
CPP_GATEWAY_PROTOTYPE(sci_insert);
CPP_GATEWAY_PROTOTYPE(sci_cell_gw);
CPP_GATEWAY_PROTOTYPE(sci_makecell);
CPP_GATEWAY_PROTOTYPE(sci_typeof);
CPP_GATEWAY_PROTOTYPE(sci_tlist_gw);
CPP_GATEWAY_PROTOTYPE(sci_mlist_gw);
CPP_GATEWAY_PROTOTYPE(sci_isfield);
CPP_GATEWAY_PROTOTYPE(sci_fieldnames);
CPP_GATEWAY_PROTOTYPE(sci_oldEmptyBehaviour);

#ifndef NDEBUG
CPP_GATEWAY_PROTOTYPE(sci_inspectorGetItemCount);
CPP_GATEWAY_PROTOTYPE(sci_inspectorShowItem);
CPP_GATEWAY_PROTOTYPE(sci_inspectorGetItem);
CPP_GATEWAY_PROTOTYPE(sci_inspectorGetUnreferencedItem);
CPP_GATEWAY_PROTOTYPE(sci_inspectorDeleteUnreferencedItems);
CPP_GATEWAY_PROTOTYPE(sci_inspectorShowUnreferencedItem);
CPP_GATEWAY_PROTOTYPE(sci_inspectorGetFunctionList);
#endif

#endif /* __CORE_GW_HXX__ */
