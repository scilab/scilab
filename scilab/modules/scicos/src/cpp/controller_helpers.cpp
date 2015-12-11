/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2015 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <string>

#include "controller_helpers.hxx"
#include "Controller.hxx"
#include "View.hxx"

namespace org_scilab_modules_scicos
{

LoggerView* get_or_allocate_logger()
{
    static const std::string loggerViewName = "logger";

    View* registeredView = Controller::look_for_view(loggerViewName);
    if (registeredView == nullptr)
    {
        registeredView = Controller::register_view(loggerViewName, new LoggerView());
    }
    LoggerView* logger = static_cast<LoggerView*>(registeredView);
    return logger;
}

} /* namespace org_scilab_modules_scicos */
