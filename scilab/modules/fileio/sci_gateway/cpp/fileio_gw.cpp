/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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

#include "fileio_gw.hxx"
#include "context.hxx"
#include "function.hxx"

#define MODULE_NAME L"fileio"

#ifdef _MSC_VER
#pragma comment(lib,"../../../../bin/libxml2.lib")
#pragma comment(lib,"../../../../bin/libcurl.lib")
#endif

extern "C"
{
#include "gw_fileio.h"
}

int FileioModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"cd", &sci_chdir, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"chdir", &sci_chdir, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"pwd", &sci_pwd, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"mopen", &sci_mopen, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"mclose", &sci_mclose, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"mput", &sci_mput, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"mget", &sci_mget, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"mputl", &sci_mputl, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"mgetl", &sci_mgetl, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"pathconvert", &sci_pathconvert, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"isdir", &sci_isdir, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"filesep", &sci_filesep, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"fileparts", &sci_fileparts, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"findfiles", &sci_findfiles, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"basename", &sci_basename, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"isfile", &sci_isfile, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"fileinfo", &sci_fileinfo, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"deletefile", &sci_deletefile, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"getshortpathname", &sci_getshortpathname, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"getlongpathname", &sci_getlongpathname, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"fileext", &sci_fileext, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"createdir", &sci_createdir, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"fullpath", &sci_fullpath, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"copyfile", &sci_copyfile, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"removedir", &sci_removedir, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"meof", &sci_meof, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"mputstr", &sci_mputstr, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"mgetstr", &sci_mgetstr, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"mseek", &sci_mseek, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"mtell", &sci_mtell, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"newest", &sci_newest, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"merror", &sci_merror, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"mclearerr", &sci_mclearerr, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"get_absolute_file_path", &sci_get_absolute_file_path, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"getrelativefilename", &sci_getrelativefilename, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"pathsep", &sci_pathsep, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"mfprintf", &sci_mfprintf, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"mscanf", &sci_mscanf, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"mfscanf", &sci_mfscanf, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"msscanf", &sci_msscanf, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"getdrives", &sci_getdrives, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"fprintfMat", &sci_fprintfMat, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"fscanfMat", &sci_fscanfMat, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"getURL", &sci_getURL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"movefile", &sci_movefile, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"splitURL", &sci_splitURL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"tempname", &sci_tempname, MODULE_NAME));
    return 1;
}
