// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function r=mtlb_exist(nam,wh)
// Emulation function for exist() Matlab function

[lhs,rhs]=argn(0)
fptr=funptr(nam)
r=0
if rhs==1 then
  if fptr<>0 then 
    fptr=int(fptr/1000)
    if fptr<=500 then
      r=5
    else
      r=3
    end
  elseif exists(nam)==1 then
    if type(nam)==11|type(nam)==13 then
      r=2
    else
      r=1
    end
  end
else
  select wh
  case "var" then
    if exists(nam)==1 then r=1,end
  case "builtin" then
    if fptr<>0 then 
      fptr=int(fptr/1000)
      if fptr<=500 then
	r=5
      else
	r=3
      end
    end
  case "file" then
  case "dir" then
  end
end
endfunction
