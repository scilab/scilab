function [tree]=%imp2sci(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab colon
// Input: tree = Matlab operation tree
// Output: tree = Scilab equivalent for tree
// Emulation function: mtlb_imp()
// V.C.

// A:B
if size(tree.operands)==2 then
  
  // Convert all inputs to double because Matlab also accept Strings...
  [A,B] = getoperands(tree)
  A = convert2double(A)
  B = convert2double(B)
  tree.operands=list(A,B)
  
  if is_empty(A) | is_empty(B) then
    set_infos("One operand is an empty matrix in : "+expression2code(tree)+", result set to []",1);
    tree=Cste([])
    tree.dims=list(1,0)
  elseif not_empty(A) & not_empty(B) then
    tree.out(1).dims=list(1,Unknown)
    tree.out(1).type=Type(Double,Real)
  else
    tree=Funcall("mtlb_imp",1,list(A,B),tree.out)
    tree.lhs(1).dims=list(1,Unknown)
    tree.lhs(1).type=Type(Double,Real)
  end
// A:inc:B
else

  // Convert all inputs to double because Matlab also accept Strings...
  [A,inc,B]=getoperands(tree)
  A = convert2double(A)
  B = convert2double(B)
  inc = convert2double(inc)
  tree.operands=list(A,inc,B)

  if is_empty(A) | is_empty(B) | is_empty(inc) then
    set_infos("One operand is an empty matrix in : "+expression2code(tree)+", result set to []",1);
    tree=Cste([])
    tree.dims=list(1,0)
  elseif not_empty(A) & not_empty(B) & not_empty(inc) then
    tree.out(1).dims=list(1,Unknown)
    tree.out(1).type=Type(Double,Real)
  else
    tree=Funcall("mtlb_imp",1,list(A,inc,B),tree.out)
    tree.lhs(1).dims=list(1,Unknown)
    tree.lhs(1).type=Type(Double,Real)
  end
end
endfunction

