function tree=sci_asec(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab asec()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

X=getrhs(tree)
X=convert2double(X)

tree.name="acos"
Op=Operation("./",list(Cste(1),X),list())
tree.rhs=Rhs(Op)

tree.lhs(1).dims=X.dims
tree.lhs(1).type=Type(Double,Unknown)
endfunction
