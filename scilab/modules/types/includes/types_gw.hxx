/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

#ifndef __TYPES_GW_HXX__
#define __TYPES_GW_HXX__


#include "dynlib_types_gw.h"

#include "funcmanager.hxx"
#include "context.hxx"


class TypesModule
{
private :
  TypesModule() {};
  ~TypesModule() {};

public :
  TYPES_GW_IMPEXP static bool Load();
};

Function::ReturnValue sci_list(types::typed_list &in, int _iRetCount, types::typed_list &out);
Function::ReturnValue sci_struct(types::typed_list &in, int _iRetCount, types::typed_list &out);
Function::ReturnValue sci_null(types::typed_list &in, int _iRetCount, types::typed_list &out);
Function::ReturnValue sci_insert(types::typed_list &in, int _iRetCount, types::typed_list &out);

#endif /* !__TYPES_GW_HXX__ */
