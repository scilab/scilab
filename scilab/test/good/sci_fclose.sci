function [tree]=sci_fclose(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab fclose()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

fid=getrhs(tree)

tree.name="mclose"

if fid.vtype==Unknown then
  set_infos(["Unknown type for input parameter: consider fclose(fid)"],2)
end

tree.lhs(1).dims=list(1,1)
tree.lhs(1).type=Type(Double,Real)

endfunction
