// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function st=%0_i_st(i,void,st)
  if type(i)==10 then
    f=getfield(1,st);
    k=find(f(3:$)==i);
    if k<>[] then
      f(k+2)=[];
      setfield(k+2,null(),st);
      setfield(1,f,st);
    else
      error(_("Invalid index.\n"))
    end
  else
    error(_("Incorrect assignment.\n"))
  end
endfunction
