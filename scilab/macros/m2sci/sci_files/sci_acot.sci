function tree=sci_acot(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab acot()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

X=getrhs(tree)
X=convert2double(X)

tree.name="atan"
Op=Operation("./",list(Cste(1),X),list())
tree.rhs=Rhs(Op)

tree.lhs(1).dims=X.dims
tree.lhs(1).type=X.type
endfunction
