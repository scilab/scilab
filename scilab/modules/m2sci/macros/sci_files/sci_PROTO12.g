function [tree]=sci_MFUN(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab MFUN()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

k=1
while k<=size(tree.rhs)
  if tree.rhs(k).value=="" then
    tree.rhs(k)=null()
  end
  k=k+1
end

if size(tree.rhs)==0 then
  // Nothing to do
else
  select tree.rhs(1).value
  case "global" then
    // Nothing to do
  case "-file" then
    no_equiv(expression2code(tree))
  else
    no_equiv(expression2code(tree))
  end
end

endfunction
