function [tree]=sci_dir(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab dir()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

if tree.lhs(1).name=="ans" then
  tree.name="dir"
else
  tree.name="mtlb_dir"
  tree.lhs(1).type=Type(Struct,Real)
  tree.lhs(1).dims=list(Unknown,1)
  tree.lhs(1).contents=struct("name",Infer(list(1,Unknown),Type(String,Real)),..
      "date",Infer(list(1,Unknown),Type(String,Real)),..
      "bytes",Infer(list(1,1),Type(Double,Real)),..
      "isdir",Infer(list(1,1),Type(Boolean,Real)))
end

endfunction
