// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_echo(tree)
// M2SCI function
// Conversion function for Matlab echo()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_echo()

// echo toggle
if rhs<=0 then
  // mode(abs(mode()-1))
  tree.name="mode"
  sub=Operation("-",list(tree,Cste(1)),list())
  abs_funcall=Funcall("abs",1,Rhs_tlist(sub),list())
  tree.rhs=Rhs_tlist(abs_funcall)
// echo on or echo off
elseif rhs==1 then
  if typeof(tree.rhs(1))=="cste" then
    if tree.rhs(1).value=="on" then
      tree.name="mode"
      tree.rhs=Rhs_tlist(1);
    elseif tree.rhs(1).value=="off" then
      tree.name="mode"
      tree.rhs=Rhs_tlist(0);
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
