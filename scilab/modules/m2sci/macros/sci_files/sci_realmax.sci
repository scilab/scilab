function tree=sci_realmax(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab realmax()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_realmax()
// V.C.

// realmax
if rhs<=0 then
  tree.name="number_properties"
  tree.rhs(1)=Cste("huge");
// realmax('double') or realmax('single')
else
  if typeof(tree.rhs(1))=="cste" then
    if tree.rhs(1).value=="double" then
      tree.name="number_properties"
      tree.rhs(1)=Cste("huge");
    else
      no_equiv(expression2code(tree));
    end
  else
    tree.name="mtlb_realmax";
  end
end
tree.lhs(1).dims=list(1,1)
tree.lhs(1).type=Type(Double,Real)
endfunction
