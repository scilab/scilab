function tree=sci_realmin(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab realmin()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_realmin()
// V.C.

// realmin
if rhs<=0 then
  tree.name="number_properties"
  tree.rhs(1)=Cste("tiny");
// realmin('double') or realmin('single')
else
  if typeof(tree.rhs(1))=="cste" then
    if tree.rhs(1).value=="double" then
      tree.name="number_properties"
      tree.rhs(1)=Cste("tiny");
    else
      no_equiv(expression2code(tree));
    end
  else
    tree.name="mtlb_realmin";
  end
end
tree.lhs(1).dims=list(1,1)
tree.lhs(1).type=Type(Double,Real)
endfunction
