function tree=sci_cond(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab cond()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_cond()
// F.B

if rhs == 1 then
  A = getrhs(tree);
  for i=1:size(A.dims)
    if A.dims(i) == -1 | A.dims(i) == 0 then
      tree.name = "mtlb_cond";
    end
  end
elseif rhs == 2
  tree.name = "mtlb_cond";
end
tree.lhs(1).type = Type(Double,Real);
tree.lhs(1).dims =list(1,1);
endfunction