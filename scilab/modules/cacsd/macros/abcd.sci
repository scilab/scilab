// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [A,B,C,D]=abcd(sl)
// Retrieves [A,B,C,D] matrices from linear system sl

if type(sl)<>16 then
  error(msprintf(gettext("%s: Wrong type for input argument #%d: Typed list expected.\n"),"abcd",1))
  return;
end
typis=sl(1);
if typis(1)=='lss' then
  [A,B,C,D]=sl(2:5)
  return;
end
if typis(1)=='r' then
  w=tf2ss(sl);
  [A,B,C,D]=w(2:5)
end
endfunction
