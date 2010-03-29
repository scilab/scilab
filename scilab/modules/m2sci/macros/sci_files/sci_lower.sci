// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_lower(tree)
// M2SCI function
// Conversion function for Matlab lower()
// Input: tree = Matlab funcall tree
// Output: tree = Scilab equivalent for tree
// Emulation function: mtlb_lower()

opt=part("lower",1)

A = getrhs(tree)
if A.vtype==String then
  tree.name="convstr"
  tree.rhs=Rhs_tlist(A,opt)
  tree.lhs(1).dims=A.dims
  tree.lhs(1).type=A.type
elseif A.vtype==Unknown then
  tree.name="mtlb_lower"
  tree.lhs(1).dims=A.dims
  tree.lhs(1).type=A.type
else
  if typeof(A)=="funcall" then
    A.lhs=tree.lhs
  elseif typeof(A)=="operation" then
    A.out=tree.lhs
  end
  tree=A
end
endfunction
