// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_asinh(tree)
// File generated from sci_PROTO4.g: PLEASE DO NOT EDIT !
// M2SCI function
// Conversion function for Matlab asinh()
// Input: tree = Matlab funcall tree
// Output: tree = Scilab equivalent for tree

tree.name="asinh"

A=getrhs(tree)
A=convert2double(A)
tree.rhs=Rhs_tlist(A)

tree.lhs(1).dims=A.dims
tree.lhs(1).type=A.type
endfunction
