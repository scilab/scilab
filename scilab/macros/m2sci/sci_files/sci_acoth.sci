function tree=sci_acoth(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab acoth()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

X=getrhs(tree)
X=convert2double(X)

set_infos(["If "+expression2code(X)+" is outside [-1,1]"
    "   complex part of output of "+expression2code(tree)+" will be the opposite of Matlab one."],2)

tree.name="atanh"
Op=Operation("./",list(Cste(1),X),list())
tree.rhs=Rhs(Op)

tree.lhs(1).dims=X.dims
// Property unknown because result can be complex for real input
tree.lhs(1).type=Type(X.vtype,Unknown) 
endfunction
