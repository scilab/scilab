// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_ishold(tree)
// M2SCI function
// Conversion function for Matlab ishold()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree

gca_funcall=Funcall("gca",1,list(),list())
get_funcall=Funcall("get",1,Rhs_tlist(gca_funcall,"auto_clear"),list())
tree=Operation("==",list(get_funcall,Cste("off")),tree.lhs)

tree.out(1).dims=list(1,1)
tree.out(1).type=Type(Boolean,Real)

endfunction
