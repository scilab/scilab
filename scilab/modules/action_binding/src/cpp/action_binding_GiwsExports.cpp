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

#include "action_binding_GiwsExports.hxx"
#include "Signal.hxx"

using namespace org_scilab_modules_action_binding_utils;
void Signal_notify(JavaVM * jvm_, char * ID)
{
    Signal::notify(jvm_, ID);
}
