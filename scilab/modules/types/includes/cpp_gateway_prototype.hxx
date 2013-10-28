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

#ifndef __CPP_GATEWAY_PROTOTYPE_HXX__
#define __CPP_GATEWAY_PROTOTYPE_HXX__

#include "types.hxx"
#include "callable.hxx"

#define CPP_GATEWAY_PROTOTYPE(__gateway__) extern "C" types::Callable::ReturnValue (__gateway__)(types::typed_list &in, int _iRetCount, types::typed_list &out)
#define CPP_GATEWAY_PROTOTYPE_EXPORT(__gateway__, __export__) extern "C" __export__ types::Callable::ReturnValue (__gateway__)(types::typed_list &in, int _iRetCount, types::typed_list &out)
#define CPP_OPT_GATEWAY_PROTOTYPE(__gateway__) extern "C" types::Callable::ReturnValue (__gateway__)(types::typed_list &in, types::optional_list &opt, int _iRetCount, types::typed_list &out)
#define CPP_OPT_GATEWAY_PROTOTYPE_EXPORT(__gateway__, __export__) extern "C" __export__ types::Callable::ReturnValue (__gateway__)(types::typed_list &in, types::optional_list &opt, int _iRetCount, types::typed_list &out)

#endif /* !__CPP_GATEWAY_PROTOTYPE_HXX__ */
