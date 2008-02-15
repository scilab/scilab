// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_isspace(tree)
// M2SCI function
// Conversion function for Matlab isspace()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_isspace()

A = getrhs(tree)

if A.vtype==String then
  A = Funcall("asciimat",1,Rhs_tlist(A),list(Variable("",A.infer)))
  tree = Operation("==",list(A,Cste(32)),tree.lhs)
  tree.out(1).dims=A.dims
  tree.out(1).type=Type(Boolean,Real)
elseif or(A.vtype==[Double,Boolean]) then
  tree.name="zeros"
  tree.lhs(1).dims=A.dims
  tree.lhs(1).type=Type(Boolean,Real)
else
  tree.name="mtlb_isspace"
  tree.lhs(1).dims=A.dims
  tree.lhs(1).type=Type(Boolean,Real)
end

endfunction
