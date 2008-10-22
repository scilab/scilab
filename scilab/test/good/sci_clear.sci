function [tree]=sci_clear(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab clear()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

for k=1:rhs
  if tree.rhs(k).value=="" then
    tree.rhs(k)=null()
  end
end

if size(tree.rhs)==0 then
  tree.lhs(1).dims=list(Unknown,Unknown)
  tree.lhs(1).type=Type(Unknown,Unknown)
else
  select tree.rhs(1).value
  case "global" then
    tree.rhs(1)=null()
    tree.name="clearglobal"
  case "all" then
    tree.rhs=list()
  case "classes" then
    no_equiv(expression2code(tree))
  case "functions" then
    no_equiv(expression2code(tree))
  case "import" then
    no_equiv(expression2code(tree))
  case "variables" then
    tree.rhs=list()
  else
    // Nothing to do
  end
  tree.lhs(1).dims=list(Unknown,Unknown)
  tree.lhs(1).type=Type(Unknown,Unknown)
end

endfunction
