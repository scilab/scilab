// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [stk,nwrk,txt,top]=%t2for(nwrk)
// genere le code fortarn relatif a la transposition
//!
s2=stk(top)
if s2(4)<>'1'&s2(5)<>'1' then
  [out,nwrk,txt]=outname(nwrk,s2(3),s2(4),s2(5))
  txt=[txt;gencall(['mtran',s2(1),s2(4),out,s2(5),s2(4),s2(5)])];
  stk=list(out,'-1',s2(3),s2(5),s2(4))
else
  stk=list(s2(1),s2(2),s2(3),s2(5),s2(4))
end
endfunction
