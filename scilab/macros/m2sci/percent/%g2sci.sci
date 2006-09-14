function [tree]=%g2sci(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab logical OR
// Input: tree = Matlab operation tree
// Output: tree = Scilab equivalent for tree
// V.C.

// Overloading functions in $SCI/macros/mtlb/:
// - %b_g_s.sci
// - %s_g_b.sci
// These functions are not used to get the same output value as Matlab one with empty matrices

// %s_g_s is also defined but no more used (hard coded)

[A,B] = getoperands(tree)

// Short circuiting OR
if (typeof(B)=="variable" & B.name=="%shortcircuit") then
  if typeof(tree.out(1))=="variable" & tree.out(1).name=="ans" then
    tmp=gettempvar()
    tmp.type=Type(Double,Real)
    tree=tmp
  else
    tmp=tree.out(1)
    global("varslist")
    varslist($+1)=M2scivar(tree.out(1).name,tree.out(1).name,Infer(list(1,1),Type(Boolean,Real)))
    tree=list()
  end
  insert(Equal(list(tmp),Funcall("bool2s",1,list(Cste(%T)),list())))
  insert(tlist(["ifthenelse","expression","then","elseifs","else"],Operation("~",list(A.operands(1)),list()),list(Equal(list(tmp),A.operands(2))),list(),list()))
  return
end

// To have good size for result with String as input
// And overloading functions are not written for Strings
A = convert2double(A)
B = convert2double(B)
tree.operands=list(A,B)

tree.out(1).type=Type(Boolean,Real)
// If A is a scalar
if is_a_scalar(A) then
  tree.out(1).dims=B.dims
// If B is a scalar
elseif is_a_scalar(B) then
  tree.out(1).dims=A.dims
// If A or B is an empty matrix  
elseif is_empty(A) | is_empty(B) then
  tree.out(1).dims=A.dims
// A and B are not scalars and not empty matrices -> they have the same size
elseif not_empty(A) & not_empty(B) then
  tree.out(1).dims=A.dims
else
  tree.out(1).dims=allunknown(A.dims)
end

endfunction








