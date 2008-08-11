// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function mtlb_echo(arg1,arg2)
// Emulation function for Matlab echo()

rhs=argn(2);

if rhs<=0 then
  mode(abs(mode()-1))
elseif rhs==1 then
  if arg1=="on" then
    mode(1);
  elseif arg1=="off" then
    mode(0);
  else
    error(msprintf(gettext("%s: Not yet implemented.\n"),"mtlb_echo"));
  end
else
    error(msprintf(gettext("%s: Not yet implemented.\n"),"mtlb_echo"));
end
endfunction
