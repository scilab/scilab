function tree=sci_dec2bin(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab dec2bin()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// F.B

proddims=1
for k=tree.rhs(1).dims
 proddims=k*proddims
end
tree.lhs(1).dims=list(proddims,1)
tree.lhs(1).type=Type(String,Real)
endfunction