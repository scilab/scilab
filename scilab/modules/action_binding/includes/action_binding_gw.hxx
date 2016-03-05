/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2011 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
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

#ifndef __ACTION_BINDING_GW_HXX__
#define __ACTION_BINDING_GW_HXX__

#include "cpp_gateway_prototype.hxx"

extern "C"
{
#include "dynlib_action_binding_gw.h"
}

CPP_GATEWAY_PROTOTYPE_EXPORT(sci_notify, ACTION_BINDING_GW_IMPEXP);

#endif /* !__ACTION_BINDING_GW_HXX__ */
