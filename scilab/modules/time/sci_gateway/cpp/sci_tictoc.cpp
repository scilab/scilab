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

#include "time_gw.hxx"
#include "function.hxx"
#include "callable.hxx"
#include "double.hxx"
#include "timer.hxx"

static Timer timer;

types::Function::ReturnValue sci_tic(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    timer.start();
    return types::Function::OK;
}

types::Function::ReturnValue sci_toc(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    // timer will give time in millis
    // Scilab used to give it in seconds ...
    out.push_back(new types::Double(timer.elapsed_time() / 1000.0));
    return types::Function::OK;
}
