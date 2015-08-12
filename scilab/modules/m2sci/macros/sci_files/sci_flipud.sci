// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_flipud(tree)
    // M2SCI function
    // Conversion function for Matlab flipud()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    treedims=tree.rhs(1).dims
    treetype=tree.rhs(1).type


    if typeof(tree.rhs(1))<>"variable" then
        tmp=gettempvar()
        insert(Equal(list(tmp),tree.rhs(1)))
    else
        tmp=tree.rhs(1)
    end

    ind1=Operation(":",list(Cste("$"),Cste(-1),Cste(1)),list())
    ind2=Cste(":")

    tree=Operation("ext",list(tmp,ind1,ind2),tree.lhs)

    tree.out(1).dims=treedims
    tree.out(1).type=treetype
endfunction
