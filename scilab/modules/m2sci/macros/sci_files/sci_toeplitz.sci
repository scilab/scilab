function [tree]=sci_toeplitz(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab toeplitz()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

// T = toeplitz(c)
if rhs==1 then
  c=convert2double(tree.rhs(1))
  if c.property<>Real then
    tree.name="mtlb_toeplitz"
  elseif ~is_a_vector(c) & ~is_a_scalar(c) then
    tree.name="mtlb_toeplitz"
  end
  tree.rhs(1)=c
  tree.lhs(1).type=Type(Double,c.property)
// T = toeplitz(c,r)
else
  [c,r]=getrhs(tree)
  tree.rhs=Rhs(convert2double(c),convert2double(r))
  tree.name="mtlb_toeplitz"
  if c.property==Complex | r.property==Complex then
    tree.lhs(1).type=Type(Double,Complex)
  elseif c.property==Real | r.property==Real then
    tree.lhs(1).type=Type(Double,Real)
  else
    tree.lhs(1).type=Type(Double,Unknown)
  end
end

endfunction
