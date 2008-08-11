// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [vd,D]=mtlb_eig(A,B)
// Emulation function for eig() Matlab function

[lhs,rhs]=argn()

if rhs==1 then
  if lhs==1 then
    vd=spec(A)
  else
    [vd,D]=spec(A)
    if isreal(vd,0) & size(vd,"*")>1 then
      vd(:,1:2)=-vd(:,1:2)
    end
  end  
else
  error(msprintf(gettext("%s: use with %d input argument(s) is not yet implemented.\n"),"mtlb_eig",2));
end
endfunction
