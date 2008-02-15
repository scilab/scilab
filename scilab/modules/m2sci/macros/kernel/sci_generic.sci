// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_generic(tree)
// M2SCI function
// Generic conversion function for unknown Matlab functions
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree

if typeof(tree)=="operation"
  tree.out(1).dims=list(-1,-1)
  tree.out(1).type=Type(-1,-1)
else
  for i=1:size(tree.lhs)
    tree.lhs(i).dims=list(-1,-1)
    tree.lhs(i).type=Type(-1,-1)
  end
end
endfunction
