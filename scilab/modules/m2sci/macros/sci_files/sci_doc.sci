function [tree]=sci_doc(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab doc()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

if rhs==0 then
  tree.rhs=list()
else
  topic=getrhs(tree)
  k=strindex(topic.value,"/")
  if k<>[] & min(k)<>2 then // help toolbox/
    no_equiv(expression2code(tree));
  elseif topic.value=="syntax" then
    tree.rhs=Rhs("names")
  else
    // Nothing to do
  end
end

endfunction
