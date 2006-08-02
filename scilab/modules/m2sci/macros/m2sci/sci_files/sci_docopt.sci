function [tree]=sci_docopt(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab docopt()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

tree=Variable("%browsehelp",Infer(list(1,Unknown),Type(String,Real)))
insert(Equal(list(),Funcall("global",1,list(Cste(tree.name)),list())))
endfunction
