function [tree]=sci_load(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab load()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.
global("varslist")
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
rhstemp=list()
rhstemp($+1)=tree.rhs(1)

endfunction
