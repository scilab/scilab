// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_pause(tree)
// M2SCI function
// Conversion function for Matlab pause
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree

if rhs<1 then
  tree.name="halt"
else
  n = getrhs(tree)
  if n.vtype==String then // pause on/off
    no_equiv(expression2code(tree));
  else
    tree.rhs=Rhs_tlist(Operation("*",list(Cste(1000),n),list()))
    tree.name="xpause"
  end
end

endfunction
