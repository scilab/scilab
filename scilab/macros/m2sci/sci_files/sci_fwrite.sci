function [tree]=sci_fwrite(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab fwrite()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_fwrite()
// V.C.

set_infos(["No simple equivalent, so mtlb_fwrite() is called"],0);

if rhs==4 then
  no_equiv("fwrite() called with skip parameter");
end

tree.name="mtlb_fwrite"

tree.lhs(1).dims=list(1,1)
tree.lhs(1).type=Type(Double,Real)
endfunction
