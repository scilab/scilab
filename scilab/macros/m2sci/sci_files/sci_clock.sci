function [tree]=sci_clock(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab clock()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

w = gettempvar()
elt1=Operation(":",list(Cste(1),Cste(2)),list())
elt2=Operation(":",list(Cste(6),Cste(9)),list())
row=Operation("rc",list(elt1,elt2),list())

insert(Equal(list(w),Funcall("getdate",1,list(),list())))

tree=Operation("ext",list(w,row),tree.lhs)

tree.out(1).dims=list(1,6)
tree.out(1).type=Type(Double,Real)
endfunction
