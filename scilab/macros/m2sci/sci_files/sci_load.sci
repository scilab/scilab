function [tree]=sci_load(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab load()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

tree.name="loadmatfile"

// If no rhs, then load all environnement
if rhs<=0 then
  tree.name="loadmatfile"
  tree.rhs=Rhs("scilab.mat")
  return
end

for k=1:rhs
  if typeof(tree.rhs(k))=="cste" & tree.rhs(k).value=="-regexp" then
    set_infos(["Option -regexp not yet handled: will be ignored"],2);
  end
end

endfunction
