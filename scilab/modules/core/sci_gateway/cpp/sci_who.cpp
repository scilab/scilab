/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "core_gw.hxx"
#include "function.hxx"
#include "context.hxx"
#include "scilabWrite.hxx"

using namespace types;

Function::ReturnValue sci_who(typed_list& in, int iRetCount, typed_list& out)
{
    std::wstringstream wstream;
    wstream << *(symbol::Context::getInstance()) << std::endl;

    scilabForcedWriteW(wstream.str().c_str());

    return Function::OK;
}
