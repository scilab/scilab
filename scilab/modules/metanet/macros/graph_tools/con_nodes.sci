function ns=con_nodes(i,g)
// Copyright INRIA
  if argn(2)<>2 then error(39), end
  // check i
  if prod(size(i))<>1 then
    error('First argument must be a scalar')
  end
  // checking of g is made in connex
  [l,nc]=connex(g)
  ns=find(nc==i)
endfunction
