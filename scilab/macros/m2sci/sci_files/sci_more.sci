function [tree]=sci_more(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab more()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_more()
// V.C.
opt=getrhs(tree)

tree.name="lines"
if typeof(opt)=="cste" then
  if opt.value=="off" then
    tree.rhs=Rhs(0)
  elseif opt.value=="on" then
    tree.rhs=Rhs(60)
  else
    // Nothing to do
  end
else
  tree.name="mtlb_more"
end

endfunction
