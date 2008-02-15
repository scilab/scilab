// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Farid BELAHCENE
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_logm(tree)
// Generic conversion function
// M2SCI function
// Conversion function for Matlab logm()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree

A=getrhs(tree)
tree.rhs=Rhs_tlist(A)

tree.lhs(1).dims=A.dims
tree.lhs(1).type=Type(Double,Unknown)
if size(tree.lhs)==2
  tree.lhs(2)=null()
  set_infos(gettext("second output argument will be ignored."),2);
end
endfunction




