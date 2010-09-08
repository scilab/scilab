// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=%i2sci_s(tree)
// M2SCI function
// Conversion function for Matlab insertion in scalars (called by %i2sci())
// Input: tree = Matlab operation tree
// Output: tree = Scilab equivalent for tree

from=tree.operands($)
to=tree.operands(1)
ind=tree.operands(2)

tree.operands(2)=list(Cste(1),ind)
if ~is_a_scalar(from) & from.dims(1)<>1 then
  tree.operands($)=Funcall("matrix",1,Rhs_tlist(from,1,Operation("-",list(Cste(1)),list())),list())
end

// Data inference
if typeof(ind)=="cste" then
  tree.out(1).dims=list(1,ind.value)
else
  tree.out(1).dims=list(1,Unknown)
end
tree.out(1).type=to.type
endfunction

  