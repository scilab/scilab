/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
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

#include "core_gw.hxx"
#include "context.hxx"

#define MODULE_NAME L"core"
extern "C"
{
#include "gw_core.h"
}

int CoreModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"type", &sci_type, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"getmodules", &sci_getmodules, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"getdebuginfo", &sci_getdebuginfo, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"getversion", &sci_getversion, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"clear", &sci_clear, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"banner", &sci_banner, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"quit", &sci_quit, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"exit", &sci_exit, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"exists", &sci_exists, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"isdef", &sci_isdef, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"argn", &sci_argn, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"global", &sci_global, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"clearglobal", &sci_clearglobal, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"isglobal", &sci_isglobal, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"getos", &sci_getos, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"getscilabmode", &sci_getscilabmode, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"with_module", &sci_with_module, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"lasterror", &sci_lasterror, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"warning", &sci_warning, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"getmd5", &sci_getmd5, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"error", &sci_error, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"pause", &sci_pause, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"abort", &sci_abort, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"who", &sci_who, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"sciargs", &sci_sciargs, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"format", &sci_format, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"ieee", &sci_ieee, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"typename", &sci_typename, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"funcprot", &sci_funcprot, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"mode", &sci_mode, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"what", &sci_what, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"where", &sci_where, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"getmemory", &sci_getmemory, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"errclear", &sci_errclear, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"intppty", &sci_intppty, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"newfun", &sci_newfun, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"clearfun", &sci_clearfun, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"funptr", &sci_funptr, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"analyzerOptions", &sci_analyzeroptions, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"macr2tree", &sci_macr2tree, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"predef", &sci_predef, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"debug", &sci_debug, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"recursionlimit", &sci_recursionlimit, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"list", &sci_list_gw, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"struct", &sci_struct_gw, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"null", &sci_null, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"insert", &sci_insert, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"cell", &sci_cell_gw, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"makecell", &sci_makecell, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"typeof", &sci_typeof, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"tlist", &sci_tlist_gw, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"mlist", &sci_mlist_gw, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"isfield", &sci_isfield, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"fieldnames", &sci_fieldnames, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"oldEmptyBehaviour", &sci_oldEmptyBehaviour, MODULE_NAME));

#ifndef NDEBUG
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"inspectorGetItemCount", &sci_inspectorGetItemCount, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"inspectorShowItem", &sci_inspectorShowItem, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"inspectorShowUnreferencedItem", &sci_inspectorShowUnreferencedItem, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"inspectorGetItem", &sci_inspectorGetItem, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"inspectorGetUnreferencedItem", &sci_inspectorGetUnreferencedItem, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"inspectorDeleteUnreferencedItems", &sci_inspectorDeleteUnreferencedItems, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"inspectorGetFunctionList", &sci_inspectorGetFunctionList, MODULE_NAME));
#endif
    return 1;
}
