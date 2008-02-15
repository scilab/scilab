// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_isscalar(tree)
// M2SCI function
// Conversion function for Matlab isscalar()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree

lgth_funcall=Funcall("length",1,tree.rhs,list())
sum_funcall=Funcall("sum",1,Rhs_tlist(lgth_funcall),list())
tree=Operation("==",list(sum_funcall,Cste(1)),tree.lhs)

tree.out(1).dims=list(1,1)
tree.out(1).type=Type(Boolean,Real)

endfunction
