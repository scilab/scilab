// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function dim=firstnonsingleton(in,opt)
// opt="str" then return "r" or "c" if possible
// opt="num" then return numerical values
rhs=argn(2)
if rhs==1 then
  opt="str"
end

if opt=="num" then
  dim=1
else
  dim="r"
end
sz=size(in)
for k=1:size(sz,"*")
  if sz(k)==0 then
    if opt=="num" then
      dim=1
    else
      dim="r"
    end
    break
  elseif sz(k)>1 then
    if k==1 then
      if opt=="num" then
      	dim=1
      else
      	dim="r"
      end
    elseif k==2 then
      if opt=="num" then
      	dim=2
      else
      	dim="c"
      end
    else
      dim=sz(k)
    end
    break
  end
end

endfunction
