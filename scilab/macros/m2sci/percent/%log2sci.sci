function [tree]=%log2sci(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab logical operators
// Input: tree = Matlab operation tree
// Output: tree = Scilab equivalent for tree
// Emulation function: mtlb_logic()
// V.C.

[A,B]=getoperands(tree)
A=convert2double(A)
B=convert2double(B)

// Special case for nargout
if typeof(A)=="variable" & typeof(B)=="cste" then
  if A.name=="nargout" & B.value==0 then
    B=Cste(1)
  end
end
if typeof(B)=="variable" & typeof(A)=="cste" then
  if B.name=="nargout" & A.value==0 then
    A=Cste(1)
  end
end

tree.operands=list(A,B)

tree.out(1).type=Type(Boolean,Real)


  // Cases with empty matrix
if is_empty(A) | is_empty(B) then 
    // For >, <, >= and <= : Scilab gives an error message if both operands are []
    // For == and ~= : Scilab returns %T or %F
    set_infos("At lest one operand is an empty matrix for operator: "+expression2code(tree)+", result set to []",1);
    tree=Cste([])
elseif is_a_scalar(A) & not_empty(B) then
    tree.out(1).dims=B.dims
elseif is_a_scalar(B) & not_empty(A) then
    tree.out(1).dims=A.dims
elseif not_empty(A) & not_empty(B) then
    tree.out(1).dims=A.dims
else
    tree=Funcall("mtlb_logic",1,list(A,Cste(tree.operator),B),tree.out)
    tree.lhs(1).dims=A.dims
end
endfunction
