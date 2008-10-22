// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [stk,nwrk,txt]=%cc2for(nwrk)
//
//!
rhs=abs(evstr(op(3)))
nc='0';typ=0
for k=1:rhs
 sk=stk(top-rhs+k)
 typ=maxi(typ,evstr(sk(3)))
 nc=addf(nc,sk(5))
end
nl=sk(4)
typ=string(typ)

[out,nwrk,txt]=outname(nwrk,typ,nl,nc)
lout=length(out)
nc='0'
for k=1:rhs
  sk=stk(top-rhs+k)
  if part(out,lout)==')' then
    o1=part(out,1:lout-1)+'+'+mulf(nl,nc)+')'
  else
    o1=out+'(1,'+nc+')'
  end
  txt=[txt;gencall(['dcopy',mulf(nl,sk(5)),sk(1),'1',o1,'1'])];
  nc=addf(nc,sk(5))
end
stk=list(out,'-1',typ,nl,nc)
endfunction
