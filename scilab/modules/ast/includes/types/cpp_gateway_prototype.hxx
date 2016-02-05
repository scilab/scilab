/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
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

#ifndef __CPP_GATEWAY_PROTOTYPE_HXX__
#define __CPP_GATEWAY_PROTOTYPE_HXX__

#include "types.hxx"
#include "callable.hxx"

#define CPP_GATEWAY_PROTOTYPE(__gateway__) types::Callable::ReturnValue (__gateway__)(types::typed_list &in, int _iRetCount, types::typed_list &out)
#define CPP_GATEWAY_PROTOTYPE_EXPORT(__gateway__, __export__) extern "C" __export__ types::Callable::ReturnValue (__gateway__)(types::typed_list &in, int _iRetCount, types::typed_list &out)
#define CPP_OPT_GATEWAY_PROTOTYPE(__gateway__) types::Callable::ReturnValue (__gateway__)(types::typed_list &in, types::optional_list &opt, int _iRetCount, types::typed_list &out)
#define CPP_OPT_GATEWAY_PROTOTYPE_EXPORT(__gateway__, __export__) extern "C" __export__ types::Callable::ReturnValue (__gateway__)(types::typed_list &in, types::optional_list &opt, int _iRetCount, types::typed_list &out)

#endif /* !__CPP_GATEWAY_PROTOTYPE_HXX__ */
