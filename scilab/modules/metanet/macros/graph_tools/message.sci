
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function num=message(strings ,buttons)
//interface to message primitive to allow simple overloading for live demo 
  [lhs,rhs]=argn(0)
  if rhs==2 then
    num=messagebox(strings,"modal","scilab",buttons);
  else
    num=1
    messagebox(strings,"modal");
  end
endfunction
