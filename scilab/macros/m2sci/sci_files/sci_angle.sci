function tree=sci_angle(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab angle()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

A=getrhs(tree)
A=convert2double(A)

if typeof(A)=="variable" then
  n=A
else
  n=gettempvar()
  insert(Equal(list(n),A))
end

realpart=Funcall("real",1,list(n),list())
imagpart=Funcall("imag",1,list(n),list())

tree=Funcall("atan",1,list(imagpart,realpart),tree.lhs)

tree.lhs(1).dims=A.dims
tree.lhs(1).type=Type(Double,Real)
endfunction
