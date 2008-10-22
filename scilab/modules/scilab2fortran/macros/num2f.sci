// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [stk,top]=num2f(val,stk)
// traduit la definition d'un nombre
//!
top=top+1
nn=length(val);typ='0'
for k=1:nn
  if part(val,k)=='.' then typ='1',break,end
end
stk(top)=list(val,'0',typ,'1','1')
endfunction
