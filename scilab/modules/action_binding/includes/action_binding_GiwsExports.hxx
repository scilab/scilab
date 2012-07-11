/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2011 - Antoine ELIAS
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __ACTION_BINDING_GIWSEXPORTS_HXX__
#define __ACTION_BINDING_GIWSEXPORTS_HXX__
#include "getScilabJavaVM.h"

extern "C"
{
#include "dynlib_action_binding.h"
}

ACTION_BINDING_IMPEXP void Signal_notify(JavaVM * jvm_, char * ID);

#endif /* !__ACTION_BINDING_GIWSEXPORTS_HXX__ */