function [tree]=sci_echo(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab echo()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_echo()
// V.C.

// echo toggle
if rhs<=0 then
  // mode(abs(mode()-1))
  tree.name="mode"
  sub=Operation("-",list(tree,Cste(1)),list())
  abs_funcall=Funcall("abs",1,Rhs(sub),list())
  tree.rhs=Rhs(abs_funcall)
// echo on or echo off
elseif rhs==1 then
  if typeof(tree.rhs(1))=="cste" then
    if tree.rhs(1).value=="on" then
      tree.name="mode"
      tree.rhs=Rhs(1);
    elseif tree.rhs(1).value=="off" then
      tree.name="mode"
      tree.rhs=Rhs(0);
    else
      no_equiv(expression2code(tree))
    end
  else
    tree.name="mtlb_echo"
  end
// echo function on/off
else
  no_equiv(expression2code(tree))
end
endfunction
