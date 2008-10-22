// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function t=callfun(arg,typ)
//permet de generer le code fortran relatif a l'appel d'une fonction et
//gere la table des fonctions dont il faut specifier le type dans les
//declarations
//!

[lhs,rhs]=argn(0)
n=prod(size(arg))
t=' '+arg(1)+'('
if n==0 then return,end
for k=2:n-1,
  t=t+arg(k)+','
end
t=t+arg(n)+')'
//
if rhs==3 then
  nwrk;
  select typ
  case '0' then
    if find(arg(1)==nwrk(14))==[] then  nwrk(14)=[nwrk(14);arg(1)],end
  case '1' then
    if find(arg(1)==nwrk(15))==[] then  nwrk(15)=[nwrk(15);arg(1)],end
   end
end
nwrk=resume(nwrk)
endfunction
