function [tree]=sci_fread(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab fread()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_fread()
// V.C.

RHS=[]

set_infos(msprintf(gettext("No simple equivalent, so %s() is called."),"mtlb_fread"),0);

if rhs==4 then
  no_equiv(msprintf(gettext("%s() called with skip parameter."),"fread"));
end

tree.name="mtlb_fread"

if lhs==1 then
  tree.lhs(1).dims=list(1,1)
  tree.lhs(1).type=Type(Double,Real)
else
  tree.lhs(1).dims=list(Unknown,Unknown)
  tree.lhs(1).type=Type(Double,Real)
  tree.lhs(2).dims=list(1,1)
  tree.lhs(2).type=Type(Double,Real)
end
endfunction
