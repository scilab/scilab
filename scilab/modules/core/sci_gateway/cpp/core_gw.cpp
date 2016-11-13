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

#define MODULE_NAME "core"
extern "C"
{
#include "gw_core.h"
}

int CoreModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("type", &sci_type, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("getmodules", &sci_getmodules, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("getdebuginfo", &sci_getdebuginfo, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("getversion", &sci_getversion, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("clear", &sci_clear, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("banner", &sci_banner, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("quit", &sci_quit, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("exit", &sci_exit, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("exists", &sci_exists, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("isdef", &sci_isdef, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("argn", &sci_argn, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("global", &sci_global, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("clearglobal", &sci_clearglobal, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("isglobal", &sci_isglobal, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("getos", &sci_getos, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("getscilabmode", &sci_getscilabmode, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("with_module", &sci_with_module, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("lasterror", &sci_lasterror, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("warning", &sci_warning, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("getmd5", &sci_getmd5, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("error", &sci_error, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("pause", &sci_pause, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("abort", &sci_abort, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("who", &sci_who, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("sciargs", &sci_sciargs, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("format", &sci_format, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("ieee", &sci_ieee, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("typename", &sci_typename, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("funcprot", &sci_funcprot, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("mode", &sci_mode, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("what", &sci_what, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("where", &sci_where, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("getmemory", &sci_getmemory, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("errclear", &sci_errclear, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("intppty", &sci_intppty, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("newfun", &sci_newfun, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("clearfun", &sci_clearfun, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("funptr", &sci_funptr, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("analyzerOptions", &sci_analyzeroptions, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("macr2tree", &sci_macr2tree, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("predef", &sci_predef, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("debug", &sci_debug, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("recursionlimit", &sci_recursionlimit, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("list", &sci_list_gw, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("struct", &sci_struct_gw, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("null", &sci_null, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("insert", &sci_insert, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("cell", &sci_cell_gw, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("makecell", &sci_makecell, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("typeof", &sci_typeof, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("tlist", &sci_tlist_gw, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("mlist", &sci_mlist_gw, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("isfield", &sci_isfield, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("fieldnames", &sci_fieldnames, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("oldEmptyBehaviour", &sci_oldEmptyBehaviour, MODULE_NAME));

#ifndef NDEBUG
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("inspectorGetItemCount", &sci_inspectorGetItemCount, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("inspectorShowItem", &sci_inspectorShowItem, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("inspectorShowUnreferencedItem", &sci_inspectorShowUnreferencedItem, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("inspectorGetItem", &sci_inspectorGetItem, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("inspectorGetUnreferencedItem", &sci_inspectorGetUnreferencedItem, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("inspectorDeleteUnreferencedItems", &sci_inspectorDeleteUnreferencedItems, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("inspectorGetFunctionList", &sci_inspectorGetFunctionList, MODULE_NAME));
#endif
    return 1;
}
