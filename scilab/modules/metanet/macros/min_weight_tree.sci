function t=min_weight_tree(i,g)
// Copyright INRIA
[lhs,rhs]=argn(0)
select rhs
case 1
  if type(i)==16 then
    g=i
    i=1
  else
    error('First argument must be a number or a graph list')
  end
case 2 then
  if prod(size(i))<>1 then
    error('First argument must be a number')
  end
else
  error(39)
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
// value of weight
if g('edge_weight')==[] then
  w=zeros(1,ma)
else
  w=g('edge_weight')
end
// compute minimal spanning tree
if g('directed')==1 then
  alf=m6dmtree(i,la,lp,ln,n,w)
  t=m6prevn2st(alf,la,lp,ln)
else 
  if ma<0.5*n*n then 
    alf=m6umtree1(la,lp,ln,n,w)
  else 
    alf=m6umtree(la,lp,ln,n,w)
  end
  t=m6edge2st(alf) 
end
endfunction
