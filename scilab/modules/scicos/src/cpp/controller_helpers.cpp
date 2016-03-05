/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2015 - Scilab Enterprises - Clement DAVID
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
