function ns=con_nodes(i,g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>2 then error(39), end
// check i
if prod(size(i))<>1 then
  error('First argument must be a scalar')
end
// checking of g is made in connex
[l,nc]=connex(g)
ns=m6concom(i,nc)
endfunction
