function [tree]=sci_Unknown_Dims(tree)
// Copyright INRIA
// Variable dimensions set to Unknown
nbdims=size(tree.dims)
newdims=list()
for k=1:nbdims
  newdims(k)=-1
end
tree.lhs(1).dims=newdims
tree.lhs(1).type=tree.rhs(1).type
endfunction


