// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function r=mtlb_randn(a,opt)
// Emulation function for randn() Matlab function

[lhs,rhs]=argn()
if rhs==1 then
  if and(size(a)==[1 1]) then
    r=rand(a,a,"normal")
  else
    tmp=list()
    for k=1:size(a,"*")
      tmp(k)=a(k)
    end
    tmp($+1)="normal"
    r=rand(tmp(1:$))
  end
else
  if and(size(a)==[1 1]) then
    r=rand(a,a,"normal")
  else
    tmp=list()
    for k=1:size(a,"*")
      tmp(k)=a(k)
    end
    tmp($+1)="normal"
    r=rand(tmp(1:$))
  end
end
endfunction
