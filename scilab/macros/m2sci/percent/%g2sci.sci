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

// %s_g_s is also defined but no more used (hard coded)

// To have good size for result with String as input
// And overloading functions are not written for Strings
[A,B] = getoperands(tree)
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
