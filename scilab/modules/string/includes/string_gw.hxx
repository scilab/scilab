/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

#ifndef __STRING_GW_HXX__
#define __STRING_GW_HXX__

#include "dynlib_string_gw.h"

#include "funcmanager.hxx"
#include "context.hxx"

class StringModule
{
private :
   StringModule(){};
   ~StringModule(){};
public :
   EXTERN_STRING_GW static bool Load();
};

CPP_GATEWAY_PROTOTYPE(sci_grep);


#endif /* !__STRING_GW_HXX__ */
