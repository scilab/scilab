function [tree]=%r2sci(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab right division
// Input: tree = Matlab operation tree
// Output: tree = Scilab equivalent for tree
// V.C.

[A,B] = getoperands(tree)

// Scilab right division does not work with Strings, Booleans...
A = convert2double(A)
B = convert2double(B)

tree.operands=list(A,B)

if is_real(A) & is_real(B) then
  tree.out(1).type=Type(Double,Real)
elseif (is_real(A) & is_complex(B)) | (is_real(B) & is_complex(A)) then
  tree.out(1).type=Type(Double,Complex)
else
  tree.out(1).type=Type(Double,Unknown)
end
  
if is_a_scalar(A) then
  tree.out(1).dims=B.dims
elseif is_a_scalar(B) then
  tree.out(1).dims=A.dims
else
  // Dimensions are Unknown because can be (1,1) if both operands are equal...
  tree.out(1).dims=list(Unknown,Unknown)
end
  
endfunction

