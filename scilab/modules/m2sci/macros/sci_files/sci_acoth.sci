function tree=sci_acoth(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab acoth()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

X=getrhs(tree)
//X=convert2double(X)

set_infos(msprintf(gettext("If %s is outside [-1,1]\n   complex part of output of %s will be the opposite of Matlab one."),expression2code(X),expression2code(tree)),2)

tree.name="atanh"
Op=Operation("./",list(Cste(1),X),list())
tree.rhs=Rhs_tlist(Op)

tree.lhs(1).dims=X.dims
// Property unknown because result can be complex for real input
tree.lhs(1).type=Type(X.vtype,Unknown) 
endfunction
