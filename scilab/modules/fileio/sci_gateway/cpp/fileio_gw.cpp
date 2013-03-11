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
#include "context.hxx"
#include "function.hxx"

#define MODULE_NAME L"fileio"

#pragma comment(lib,"../../../../bin/libxml2.lib")
#pragma comment(lib,"../../../../bin/libcurl.lib")

extern "C"
{
#include "gw_fileio.h"
}

using namespace types;

int FileioModule::Load()
{
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"cd", &sci_chdir, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"chdir", &sci_chdir, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"pwd", &sci_pwd, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"mopen", &sci_mopen, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"mclose", &sci_mclose, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"mput", &sci_mput, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"mget", &sci_mget, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"mputl", &sci_mputl, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"mgetl", &sci_mgetl, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"pathconvert", &sci_pathconvert, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"isdir", &sci_isdir, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"filesep", &sci_filesep, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"fileparts", &sci_fileparts, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"findfiles", &sci_findfiles, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"basename", &sci_basename, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"isfile", &sci_isfile, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"fileinfo", &sci_fileinfo, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"deletefile", &sci_deletefile, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"getshortpathname", &sci_getshortpathname, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"getlongpathname", &sci_getlongpathname, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"fileext", &sci_fileext, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"createdir", &sci_createdir, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"fullpath", &sci_fullpath, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"copyfile", &sci_copyfile, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"removedir", &sci_removedir, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"meof", &sci_meof, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"mputstr", &sci_mputstr, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"mgetstr", &sci_mgetstr, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"mseek", &sci_mseek, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"mtell", &sci_mtell, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"newest", &sci_newest, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"merror", &sci_merror, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"mclearerr", &sci_mclearerr, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"get_absolute_file_path", &sci_get_absolute_file_path, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"getrelativefilename", &sci_getrelativefilename, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"pathsep", &sci_pathsep, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"mfprintf", &sci_mfprintf, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"mscanf", &sci_mscanf, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"mfscanf", &sci_mfscanf, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"msscanf", &sci_msscanf, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"getdrives", &sci_getdrives, MODULE_NAME));
    return 1;
}
