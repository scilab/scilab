function [node,radius]=graph_center(g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>1 then error(39), end
// compute lp, la and ls
ma=prod(size(g('tail')))
n=g('node_number')
head=g('head');
if g('directed')==1 then
  [lp,la,ln]=m6ta2lpd(g('tail'),head,n+1,n)
  // compute fe, che, fn, chn
  [fe,che,fn,chn]=chain_struct(lp,la,ln)
  // value of length
  if g('edge_length')==[] then
    len=ones(1,ma)
  else 
    mmi=min(g('edge_length')); mma=max(g('edge_length'));
    if (mmi<=0|mma<=0) then
      len=ones(1,ma)
    else
      len=g('edge_length')
    end
  end
else
  tail=[g('tail') head];head=[head g('tail')]; 
  [lp,la,ln]=m6ta2lpd(tail,head,n+1,n);
  // compute fe, che, fn, chn
  [fe,che,fn,chn]=chain_struct(lp,la,ln)
  // value of length
  if g('edge_length')==[] then
    len=ones(1,2*ma)
  else 
    mmi=min(g('edge_length')); mma=max(g('edge_length'));
    if (mmi<=0|mma<=0) then
      len=ones(1,2*ma)
    else
      len=[g('edge_length') g('edge_length')];
    end;
  end;
end
// compute center
[node,radius]=m6cent(fe,che,head,len,n+1)
endfunction
