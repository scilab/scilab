function tree=sci_csch(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab csch()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

X=getrhs(tree)
X=convert2double(X)

sinh_funcall=Funcall("sinh",1,list(X),list())
tree=Operation("./",list(Cste(1),sinh_funcall),tree.lhs)

tree.out(1).dims=X.dims
tree.out(1).type=Type(Double,X.property)
endfunction
