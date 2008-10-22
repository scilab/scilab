// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [stk,nwrk,txt]=%rc2for(nwrk)
// genere le code relatif a la concatenation en ligne des matrices
//!

rhs=abs(evstr(op(3)))
nl='0';typ=0
for k=1:rhs
 sk=stk(top-rhs+k)
 typ=maxi(typ,evstr(sk(3)))
 nl=addf(nl,sk(4))
end
nc=sk(5)

typ=string(typ)

[out,nwrk,txt]=outname(nwrk,typ,nl,nc)
lout=length(out)
nl1='0'
for k=1:rhs
  sk=stk(top-rhs+k)
  if part(out,lout)==')' then
    o1=part(out,1:lout-1)+'+'+mulf(nl1,nc)+')'
  else
    o1=out+'('+addf(nl1,'1')+',1)'
  end
  n1=sk(4)
  txt=[txt;gencall(['dmcopy',sk(1),n1,o1,nl,n1,nc])];
  nl1=addf(nl1,n1)
end
stk=list(out,'-1',typ,nl,nc)
endfunction
