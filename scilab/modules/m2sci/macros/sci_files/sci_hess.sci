// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_hess(tree)
// M2SCI function
// Conversion function for Matlab hess()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree

A = getrhs(tree)
A = convert2double(A)
tree.rhs=Rhs_tlist(A)

// H = hess(A)
if lhs==1 then
  tree.lhs(1).dims=A.dims
  tree.lhs(1).type=A.type
// [P,H] = hess(A)
else
  tree.lhs(1).dims=A.dims
  tree.lhs(1).type=Type(A.vtype,Unknown) // Real if A<>%i
  tree.lhs(2).dims=A.dims
  tree.lhs(2).type=A.type
  
end
endfunction
