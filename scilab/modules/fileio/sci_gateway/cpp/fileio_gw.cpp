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

#define MODULE_NAME "fileio"

#pragma comment(lib,"../../../../bin/libxml2.lib")
#pragma comment(lib,"../../../../bin/libcurl.lib")

extern "C"
{
#include "gw_fileio.h"
}

int FileioModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("cd", &sci_chdir, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("chdir", &sci_chdir, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("pwd", &sci_pwd, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("mopen", &sci_mopen, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("mclose", &sci_mclose, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("mput", &sci_mput, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("mget", &sci_mget, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("mputl", &sci_mputl, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("mgetl", &sci_mgetl, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("pathconvert", &sci_pathconvert, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("isdir", &sci_isdir, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("filesep", &sci_filesep, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("fileparts", &sci_fileparts, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("findfiles", &sci_findfiles, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("basename", &sci_basename, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("isfile", &sci_isfile, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("fileinfo", &sci_fileinfo, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("deletefile", &sci_deletefile, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("getshortpathname", &sci_getshortpathname, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("getlongpathname", &sci_getlongpathname, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("fileext", &sci_fileext, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("createdir", &sci_createdir, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("fullpath", &sci_fullpath, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("copyfile", &sci_copyfile, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("removedir", &sci_removedir, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("meof", &sci_meof, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("mputstr", &sci_mputstr, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("mgetstr", &sci_mgetstr, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("mseek", &sci_mseek, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("mtell", &sci_mtell, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("newest", &sci_newest, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("merror", &sci_merror, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("mclearerr", &sci_mclearerr, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("get_absolute_file_path", &sci_get_absolute_file_path, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("getrelativefilename", &sci_getrelativefilename, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("pathsep", &sci_pathsep, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("mfprintf", &sci_mfprintf, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("mscanf", &sci_mscanf, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("mfscanf", &sci_mfscanf, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("msscanf", &sci_msscanf, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("getdrives", &sci_getdrives, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("fprintfMat", &sci_fprintfMat, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("fscanfMat", &sci_fscanfMat, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("getURL", &sci_getURL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("movefile", &sci_movefile, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("splitURL", &sci_splitURL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("tempname", &sci_tempname, MODULE_NAME));
    return 1;
}
