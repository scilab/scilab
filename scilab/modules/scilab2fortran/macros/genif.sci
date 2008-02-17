// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [t]=genif(cnd,t1,t2)
//genif(cnd,txt) genere le if fortran relatif a la condition cnd et dont
//le  texte relatif  a la  partie  then est  donne  dans t1 et le  texte
//relatif a la partie else est eventuellement donne dans t2.
//!
[lhs,rhs]=argn(0)
if rhs==2&prod(size(t1))==1 then
  t=' if('+cnd+') '+t1
elseif rhs==3 then
  t=[' if('+cnd+') then';indentfor(t1);' else';indentfor(t2);' endif']
else
  t=[' if('+cnd+') then';indentfor(t1);' endif']
end
endfunction
