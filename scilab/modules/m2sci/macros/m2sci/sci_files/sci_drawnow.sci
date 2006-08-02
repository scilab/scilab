function [tree]=sci_drawnow(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab drawnow()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

set_infos("Drawing events are not queued in Scilab",0);
tree=Funcall("%comment",1,list(Cste(tree.name)),list())
endfunction
