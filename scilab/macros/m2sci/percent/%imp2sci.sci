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
 
  if A.vtype<>10
    A = convert2double(A)
  end
  if B.vtype<>10
    B = convert2double(B)
  end
  tree.operands=list(A,B)
  
  if  is_empty(A) | is_empty(B) then
    set_infos("One operand is an empty matrix in : "+expression2code(tree)+", result set to []",1);
    tree=Cste([])
    tree.dims=list(1,0)
  elseif not_empty(A) & not_empty(B) then
  
    if and([A.vtype,B.vtype]==10) then
      tree.out(1).dims=list(1,size(asciimat(A.value):asciimat(B.value),"*"))
    elseif and([typeof(A),typeof(B)]=="cste") then
      tree.out(1).dims=list(1,size(A.value:B.value,"*"))
    else
      tree.out(1).dims=list(1,Unknown)
    end
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
  if A.vtype<>10 then
    A = convert2double(A)
  end
  if B.vtype<>10 then
    B = convert2double(B)
  end
  if inc.vtype<>10 then
    inc = convert2double(inc)
  end
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

