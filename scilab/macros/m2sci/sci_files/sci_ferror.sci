function [tree]=sci_ferror(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab ferror()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

fid=getrhs(tree)

// ferror(fid,'clear')
if rhs==2 then
  tree.rhs(2)=null()
  if lhs==2 then
    no_equiv(expression2code(tree)+" with two outputs");
  else
    tree.name="mclearerr"
    tree.lhs(1).dims=list(1,Unknown)
    tree.lhs(1).type=Type(String,Real)
  end
// ferror(fid)
else
  if lhs==2 then
    no_equiv(expression2code(tree)+" with two outputs");
  else
    tree.name="merror"
    tree.lhs(1).dims=list(1,1)
    tree.lhs(1).type=Type(Double,Real)
  end
end

endfunction
