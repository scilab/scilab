function [tree]=sci_save(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab save()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

tree.name="savematfile"

// If no rhs, save Scilab environnement
if rhs<=0 then
  tree.rhs=Rhs("scilab.mat")
  return
end

for k=1:rhs
  if typeof(tree.rhs(k))=="cste" & tree.rhs(k).value=="-regexp" then
    set_infos(["Option -regexp not yet handled: will be ignored"],2);
  elseif typeof(tree.rhs(k))=="cste" & tree.rhs(k).value=="-append" then
    set_infos(["Option -regexp not yet handled: will be ignored"],2);
  end
end

endfunction
