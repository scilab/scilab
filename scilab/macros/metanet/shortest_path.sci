function [p,lp]=shortest_path(i,j,g,typ)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==3 then
  typ='arc'
else
  if rhs<>4 then error(39), end
end
// check i and j
if prod(size(i))<>1 then
  error('First argument must be a scalar')
end
if prod(size(j))<>1 then
  error('Second argument must be a scalar')
end
// check typ
  if type(typ)<>10|prod(size(typ))<>1 then
    error('Fourth argument must be a string')
  end
// check g
check_graph(g)
// compute lp, la and ln
n=g('node_number')
ma=prod(size(g('tail')))
if g('directed')==1 then
  [lp,la,ln]=m6ta2lpd(g('tail'),g('head'),n+1,n)
else
  [lp,la,ln]=m6ta2lpu(g('tail'),g('head'),n+1,n,2*ma)
end
// value of length
if g('edge_length')==[] then
  len=zeros(1,ma)
else
  len=g('edge_length')
end
// compute shortest path according to typ
select typ
case 'arc' then
  [l,v]=m6pcchna(i,lp,ln,n)
  p=m6prevn2p(i,j,v,la,lp,ln,g('directed'))
  lp=l(j)
case 'length'
  lneg=find(len<0)
  if lneg<>[] then 
    [l,v]=m6ford(i,la,len,lp,ln,n)
  else   
    if ma<0.5*n*n then 
      [l,v]=m6johns(i,la,len,lp,ln,n)
    else 
      [l,v]=m6dijkst(i,la,len,lp,ln,n)
    end
  end
  p=m6prevn2p(i,j,v,la,lp,ln,g('directed'))
  lp=l(j)
else
  error('Unknown type ""'+typ+'""')
end
endfunction
