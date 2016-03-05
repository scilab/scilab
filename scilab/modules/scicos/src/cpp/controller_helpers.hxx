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

#ifndef CONTROLLER_HELPERS_HXX_
#define CONTROLLER_HELPERS_HXX_

#include "LoggerView.hxx"


namespace org_scilab_modules_scicos
{

LoggerView* get_or_allocate_logger();

} /* namespace org_scilab_modules_scicos */

#endif /* CONTROLLER_HELPERS_HXX_ */
