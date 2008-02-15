// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_svd(tree)
// M2SCI function
// Conversion function for Matlab svd()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree

// svd(X)
if rhs==1 then
  A = getrhs(tree)
  A = convert2double(A)
  tree.rhs=Rhs_tlist(A)
// svd(A,0)
else
  A = getrhs(tree)
  A = convert2double(A)
  tree.rhs=Rhs_tlist(A,"e")
end

tree.lhs(1).dims=list(Unknown,1)
tree.lhs(1).type=Type(Double,Real)
if lhs>1 then
  tree.lhs(2).dims=list(Unknown,Unknown)
  tree.lhs(2).type=A.type
end
if lhs>2 then
  tree.lhs(3).dims=list(Unknown,Unknown)
  tree.lhs(3).type=Type(Double,Unknown)
end

endfunction
