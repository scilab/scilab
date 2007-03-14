function [tree]=%i2sci_s(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab insertion in scalars (called by %i2sci())
// Input: tree = Matlab operation tree
// Output: tree = Scilab equivalent for tree
// V.C.
from=tree.operands($)
to=tree.operands(1)
ind=tree.operands(2)

tree.operands(2)=list(Cste(1),ind)
if ~is_a_scalar(from) & from.dims(1)<>1 then
  tree.operands($)=Funcall("matrix",1,Rhs(from,1,Operation("-",list(Cste(1)),list())),list())
end

// Data inference
if typeof(ind)=="cste" then
  tree.out(1).dims=list(1,ind.value)
else
  tree.out(1).dims=list(1,Unknown)
end
tree.out(1).type=to.type
endfunction

  