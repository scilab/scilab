// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [tree]=sci_MFUN(tree)
// M2SCI function
// Conversion function for Matlab MFUN()
// Input: tree = Matlab funcall tree
// Output: tree = Scilab equivalent for tree

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
