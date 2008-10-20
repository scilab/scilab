/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SCILAB_FUNCTION_HH__
#define __SCILAB_FUNCTION_HH__

#include "alltypes.hxx"

class ScilabFunction {

public :
  enum ReturnValue {
    AllGood,
    WrongParamNumber,
    WrongParamType
  };

public :
  virtual ReturnValue call(types::typed_list &in, types::typed_list &out) = 0;


};
#endif /* !__SCILAB_FUNCTION_HH__ */
