function [tree]=sci_linspace(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab linspace()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_linspace()
// V.C.

// Boolean value: true if one rhs is a String
//str=%F

A=tree.rhs(1)
B=tree.rhs(2)

// %c_linspace and %b_linspace are not defined
if A.vtype==Complex | B.vtype==Complex then
  tree.lhs(1).vtype=Complex
elseif or(A.vtype==[String,Boolean,Unknown]) & or(B.vtype==[String,Boolean,Unknown]) then
  tree.lhs(1).vtype=Unknown
else 
  tree.lhs(1).vtype=Double
end

// y = linspace(A,B)
if rhs==2 then
  tree.rhs=Rhs(A,B)
  tree.lhs(1).dims=list(1,100);
else
// y = linspace(A,B,n)
  n=tree.rhs(3)
  tree.rhs=Rhs(A,B,n)
  if typeof(n)=="cste" then
    if isempty(n.value) then
      tree.lhs(1).dims=list(1,1)
    else
      tree.lhs(1).dims=list(1,n.value)
    end
  else
    tree.lhs(1).dims=list(1,Unknown)
  end
end

endfunction
