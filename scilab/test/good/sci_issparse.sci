// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_issparse(tree)
// M2SCI function
// Conversion function for Matlab issparse()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree

A = getrhs(tree)
A=Funcall("type",1,Rhs_tlist(A),list())
mat=Operation("rc",list(Cste(5),Cste(6)),list())
tree=Operation("==",list(A,mat),tree.lhs)
tree=Funcall("or",1,list(tree),tree.out)

tree.lhs(1).dims=list(1,1)
tree.lhs(1).type=Type(Boolean,Real)

endfunction
