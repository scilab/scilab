// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2009 - Serge Steer, INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function f=fieldnames(t)
  if or(type(t)==[16,17]) then
    f=matrix(getfield(1,t),-1,1)
    if f(1)<>'st' then //ignore type
      f=f(2:$)
    else //ignore type and dims
      f=f(3:$)
    end
  else
    f=[]
  end
endfunction
