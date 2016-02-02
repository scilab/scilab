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
