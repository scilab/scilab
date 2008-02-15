// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_det(tree)
// M2SCI function
// Conversion function for Matlab det()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree

X = getrhs(tree)

// %c_det and %b_det are not defined
X = convert2double(X)
tree.rhs=Rhs_tlist(X)

tree.lhs(1).dims=list(1,1)
tree.lhs(1).type=X.type
endfunction
