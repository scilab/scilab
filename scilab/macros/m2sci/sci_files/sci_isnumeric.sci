function [tree]=sci_isnumeric(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab isnumeric()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

// tf = or(type(A)==[1 5 8])
rc15=Operation("rc",list(Cste(1),Cste(5)),list())
rc158=Operation("rc",list(rc15,Cste(8)),list())
type_funcall=Funcall("type",1,list(tree.rhs(1)),list())
eq=Operation("==",list(type_funcall,rc158),list())
tree=Funcall("or",1,Rhs(eq),tree.lhs)

tree.lhs(1).dims=list(1,1)
tree.lhs(1).type=Type(Boolean,Real)

endfunction
