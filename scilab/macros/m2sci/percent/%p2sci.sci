function [tree]=%p2sci(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab exponent
// Input: tree = Matlab operation tree
// Output: tree = Scilab equivalent for tree
// V.C.

// Warning: Matlab bug for p^X when p is a character !

[A,B] = getoperands(tree)

// Scilab exponent does not work with Strings, Booleans...
A = convert2double(A)
B = convert2double(B)

tree.operands=list(A,B)

if is_real(A) & is_real(B) then
  tree.out(1).type=Type(Double,Real)
else
  // Output can be Complex or Real whatever are the inputs
  tree.out(1).type=Type(Double,Unknown)
end

if is_a_scalar(A) then
  tree.out(1).dims=B.dims
elseif is_a_scalar(B) then
  tree.out(1).dims=A.dims
else
  tree.out(1).dims=allunknown(A.dims)
end
  
endfunction

