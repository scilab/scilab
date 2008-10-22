// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Farid BELAHCENE
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function   [tree]=sci_cell2mat(tree)
// M2SCI function
// Conversion function for Matlab cell2mat()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree

A=getrhs(tree)
if is_empty(A) then
  tree.lhs(1).dims=list(0,0)
  tree.lhs(1).type=Type(Double,Real)
elseif size(A.dims)==2
  if A.dims(1)<> -1 & A.dims(2)<> -1 then
    lhs_dims2=0;
    for i=1:A.dims(2)
      lhs_dims2=A.contents.data(i).dims(2) + lhs_dims2
    end
    lhs_dims1=0;
    for i=1:A.dims(1)
      lhs_dims1=A.contents.data(1+(i-1)*A.dims(2)).dims(1) + lhs_dims1
    end
    tree.lhs(1).dims=list(lhs_dims1,lhs_dims2)
    tree.lhs(1).type = A.contents.data(1).type
  else
    tree.lhs(1).dims=list(-1,-1)
    tree.lhs(1).type=Type(Unknown,Unknown)
  end
else
  tree.lhs(1).dims=list(-1,-1)
  tree.lhs(1).type=Type(Unknown,Unknown)
end
endfunction 
