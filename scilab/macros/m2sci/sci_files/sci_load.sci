function [tree]=sci_load(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab load()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_load()
// V.C.

tree.name="mtlb_load"

// If no rhs, then load all environnement
if rhs<=0 then
  tree.name="mtlb_load"
  tree.rhs=Rhs("scilab.mat")
  return
end

for k=2:rhs
  if tree.rhs(k).value=="-mat" then
    set_infos(["Option -mat not yet handled: ignored"],2);
    tree.rhs(k)=null()
  end
end

endfunction
