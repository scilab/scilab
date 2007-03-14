function [tree]=sci_lookfor(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab lookfor()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

if rhs==2 then
  no_equiv(gettext("messages","m2sci_message_114"))
  tree.rhs(2)=null()
end

endfunction
