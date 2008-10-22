// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// Copyright (C) ???? - INRIA - Serge STEER 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function flag=fseek_origin(origin)
select origin
case "bof" then
  flag="set"
case "cof" then
  flag="cur"
case "eof" then
  flag="end"
case -1 then
  flag="set"
case 0 then
  flag="cur"
case 1 then
  flag="end" 
else
  error(msprintf(gettext("%s: Wrong value for input argument #%d: %s, %s, %s, ''%s'', ''%s'' or ''%s'' expected.\n"),"mseek", 1, "-1", "0", "1", "bof", "cof", "eof"));
end
endfunction
