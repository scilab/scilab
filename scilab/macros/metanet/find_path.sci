function p=find_path(i,j,g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>3 then error(39), end
// check i and j
if prod(size(i))<>1 then
  error('First argument must be a scalar')
end
if prod(size(j))<>1 then
  error('Second argument must be a scalar')
end
// check g
check_graph(g)
// compute lp, la and ls
n=g('node_number')
ma=prod(size(g('tail')))
if g('directed')==1 then
  [lp,la,ls]=m6ta2lpd(g('tail'),g('head'),n+1,n)
else
  [lp,la,ls]=m6ta2lpu(g('tail'),g('head'),n+1,n,2*ma)
end
// compute path
[l,v]=m6dfs(i,lp,ls,n)
p=m6prevn2p(i,j,v,la,lp,ls,g('directed'))
endfunction
