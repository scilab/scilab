function [stk,nwrk,txt]=%cc2for(nwrk)
//
//!
// Copyright INRIA
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
