/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "fileio_gw.hxx"

#define MODULE_NAME L"fileio"

extern "C"
{
	#include "gw_fileio.h"
}

using namespace types;

bool FileioModule::Load()
{
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"cd", &sci_chdir, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"chdir", &sci_chdir, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"pwd", &sci_pwd, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"mopen", &sci_mopen, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"mclose", &sci_mclose, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"mput", &sci_mput, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"mget", &sci_mget, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"mputl", &sci_mputl, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"mgetl", &sci_mgetl, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"pathconvert", &sci_pathconvert, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"isdir", &sci_isdir, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"filesep", &sci_filesep, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"fileparts", &sci_fileparts, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"findfiles", &sci_findfiles, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"basename", &sci_basename, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"isfile", &sci_isfile, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"fileinfo", &sci_fileinfo, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"deletefile", &sci_deletefile, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"getshortpathname", &sci_getshortpathname, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"getlongpathname", &sci_getlongpathname, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"fileext", &sci_fileext, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"createdir", &sci_createdir, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"fullpath", &sci_fullpath, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"copyfile", &sci_copyfile, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"removedir", &sci_removedir, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"meof", &sci_meof, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"mputstr", &sci_mputstr, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"mgetstr", &sci_mgetstr, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"mseek", &sci_mseek, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"mtell", &sci_mtell, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"newest", &sci_newest, MODULE_NAME));

    return true;
}
