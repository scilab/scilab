// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=%i2sci_g(tree)
// M2SCI function
// Conversion function for Matlab insertion in matrices (called by %i2sci())
// Input: tree = Matlab operation tree
// Output: tree = Scilab equivalent for tree

from=tree.operands($)
to=tree.operands(1)
ind=tree.operands(2)

newtree=Funcall("mtlb_i",1,Rhs_tlist(to,ind,from),list(to))
insert(Equal(list(to),newtree))
tree=list()
endfunction

  
 