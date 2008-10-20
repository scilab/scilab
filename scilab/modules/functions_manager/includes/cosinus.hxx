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

#ifndef __COSINUS_HH__
#define __COSINUS_HH__

#include <iostream>

#include "alltypes.hxx"
#include "scilabFunction.hxx"

class Cosinus : public ScilabFunction {

public :
  ReturnValue call(types::typed_list &in, types::typed_list &out);

public :
  ReturnValue callUntyped(types::GenericType *in, types::typed_list &out);

  ReturnValue callout(types::String *in);
  ReturnValue call(types::String *in);
  ReturnValue call(types::Int *in);
};

#endif /* !__COSINUS_HH__ */
