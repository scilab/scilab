// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_fgets(tree)
// M2SCI function
// Conversion function for Matlab fgets()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree


if rhs==2 then
  tree.name="mgetstr"
  tree.rhs=Rhs_tlist(tree.rhs(2),tree.rhs(1))
  if typeof(tree.rhs(1))=="cste" then
    tree.lhs(1).dims=list(1,tree.rhs(1).value)
  else
    tree.lhs(1).dims=list(1,Unknown)
  end
else
  tree.name="mgetl"
  tree.lhs(1).dims=list(1,Unknown)
  tree.rhs(2)=Cste(1)
end
tree.lhs(1).type=Type(String,Real)
endfunction
