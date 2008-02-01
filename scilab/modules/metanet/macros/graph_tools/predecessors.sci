function [a]=predecessors(i,g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>2 then error(39), end
// check g
if g('directed')<>1 then
  error('The graph must be directed')
end
n=g('node_number');
// check i
if (i<1|i>n) then
  error(string(i)+' is not a node number')
end
ta=g('tail');he=g('head');
[ir,ic]=find(he==i);
a=ta(ic);
endfunction
