function [tree]=sci_strrep(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab strrep()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_strrep()
// V.C.

[s1,s2,s3] = getrhs(tree)

if s1.vtype==String & s2.vtype==String & s3.vtype==String then
  tree.name="strsubst"
  tree.lhs(1).dims=list(Unknown,Unknown)
  tree.lhs(1).type=Type(String,Real)
else
  scitree=tree
  scitree.name="mtlb_strcmp"
  tree.name="strsubst"
  
  repl_poss(scitree,..
      tree,list(s1,s2,s3),"are character string matrices")
  
  tree=scitree
  tree.out(1).dims=list(Unknown,Unknown)
  tree.out(1).type=Type(Unknown,Unknown)
end
endfunction
