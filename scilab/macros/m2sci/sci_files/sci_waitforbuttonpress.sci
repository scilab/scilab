function [tree]=sci_waitforbuttonpress(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab waitforbuttonpress()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

btn=gettempvar()
xc=gettempvar()
yc=gettempvar()
win=gettempvar()
str=gettempvar()

insert(Equal(list(btn,xc,yc,win,str),Funcall("xclick",1,list(),list())))

comp=Operation(">",list(btn,Cste(64)),list())

tree=Funcall("bool2s",1,list(comp),tree.lhs)

tree.lhs(1).dims=list(1,1)
tree.lhs(1).type=Type(Double,Real)

endfunction
