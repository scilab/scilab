// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Farid BELAHCENE
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function  [tree]=sci_cellstr(tree)
// M2SCI function
// Conversion function for Matlab cellstr()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree

A=getrhs(tree)
tree.rhs=Rhs_tlist(A)
Aknowndim=%t // Aknowndim is a boolean, true if all dimennsions of A are known
tree.lhs(1).dims=list()
for i=1:size(A.dims) 
  if A.dims(i)==-1 then
    Aknowndim=%f
    break
  end
end
if ~Aknowndim then
  tree.lhs(1).dims(1)=-1
  tree.lhs(1).dims(2)=1
else
  tree.lhs(1).dims(1)=A.dims(1)
  tree.lhs(1).dims(2)=1
end
tree.lhs(1).type=Type(Cell,Unknown)
endfunction
