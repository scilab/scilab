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
  scitree=tree
  scitree.name="mtlb_more"
  repl1=tree
  repl1.rhs=Rhs(0)
  repl2=tree
  repl2.rhs=Rhs(60)
  
  repl_poss(scitree,..
      repl1,opt,"is ""off""",..
      repl2,opt,"is ""on""",..
      tree,opt,"is a Double value")
  tree=scitree
end

endfunction
