// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function tree=sci_realmin(tree)
// M2SCI function
// Conversion function for Matlab realmin()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_realmin()

// realmin
if rhs<=0 then
  tree.name="number_properties"
  tree.rhs(1)=Cste("tiny");
// realmin('double') or realmin('single')
else
  if typeof(tree.rhs(1))=="cste" then
    if tree.rhs(1).value=="double" then
      tree.name="number_properties"
      tree.rhs(1)=Cste("tiny");
    else
      no_equiv(expression2code(tree));
    end
  else
    tree.name="mtlb_realmin";
  end
end
tree.lhs(1).dims=list(1,1)
tree.lhs(1).type=Type(Double,Real)
endfunction
