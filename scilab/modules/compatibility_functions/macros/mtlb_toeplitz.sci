// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function z=mtlb_toeplitz(x,y)
// Emulation function for Matlab toeplitz()

if argn(2)==1 then
  if isreal(x) then
    if find(size(x)==1)==[] then
      tmp=toeplitz(x);
      s=size(x)
      z=tmp(1:s($),:)
    else
      z=toeplitz(x);
    end
    return
  end
  y=x
  x=conj(x)
  x(1)=conj(x(1))
end
if x(1)<>y(1) then
  y(1)=x(1)
end
if find(size(x)==1)==[] then
  tmp=toeplitz(x,y);
  s=size(x)
  z=tmp(1:s($),:)
else
  z=toeplitz(x,y);
end

endfunction
