/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __INTEGER_GW_HXX__
#define __INTEGER_GW_HXX__

#include "funcmanager.hxx"
#include "context.hxx"

#include "dynlib_integer_gw.h"

class IntegerModule
{
private :
	IntegerModule(){};
	~IntegerModule(){};
public :
	INTEGER_GW_IMPEXP static bool Load();
};

Function::ReturnValue sci_inttype(types::typed_list &in, int _iRetCount, types::typed_list &out);

#endif /* __INTEGER_GW_HXX__ */
