function tree=sci_legendre(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab legendre()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_legendre()
// F.B.

tree.name = "mtlb_legendre";
boolknowndims = %t;
dims = list();
for i=1:size(tree.rhs(2).dims)
  if tree.rhs(2).dims(i) == -1 then
    boolknowndims = %f;
    break
  end
  dims($+1) = tree.rhs(2).dims(i);
end

if boolknowndims then
  if typeof(tree.rhs(1)) == "cste"
    if tree.rhs(1).value > 0 then
      dims = list(tree.rhs(1).value+1,dims)
    end 
  else 
    dims = list(-1,-1)
  end
  else
    dims = list(-1,-1)
end

tree.lhs(1).dims = dims;
tree.lhs(1).type = Type(Double,Real);

endfunction
