function m=arc_number(g)
// Copyright INRIA
  if argn(2)<>1 then error(39), end
  ma=edge_number(g)
  if g.directed then m=ma, else m=2*ma, end
endfunction
