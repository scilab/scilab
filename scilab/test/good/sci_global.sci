function [tree]=sci_global(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab global()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

// Global variable for M2SCI
global("varslist");

for k=1:rhs
  nam=tree.rhs(k).value
  if nam<>"" then
    exist=%F
    for l=1:size(varslist)
      if varslist(l).matname==nam then
	exist=%T
	break
      end
    end
    if exist==%F then
      varslist($+1)=M2scivar(nam,nam,Infer(list(Unknown,Unknown),Type(Unknown,Unknown)))
      write(%io(2),"Global variable "+nam+" added to M2SCI variable list.")
    end
  end
end

endfunction
