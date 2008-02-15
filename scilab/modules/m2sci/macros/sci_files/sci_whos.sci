// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_whos(tree)
// M2SCI function
// Conversion function for Matlab whos()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree

k=1
while k<=size(tree.rhs)
  if tree.rhs(k).value=="" then
    tree.rhs(k)=null()
  end
  k=k+1
end

if size(tree.rhs)==0 then
  // Nothing to do
else
  select tree.rhs(1).value
  case "global" then
    // Nothing to do
  case "-file" then
    no_equiv(expression2code(tree))
  else
    no_equiv(expression2code(tree))
  end
end

endfunction
