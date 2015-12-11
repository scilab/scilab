// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Farid BELAHCENE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_perms(tree)
    // M2SCI function
    // Conversion function for Matlab perms()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    A=getrhs(tree)
    tree.rhs=Rhs_tlist(A)

    if (type(tree.rhs(1).dims(1))==1 & tree.rhs(1).dims(1)==1) & (type(tree.rhs(1).dims(2))==1 & tree.rhs(1).dims(2)<>-1) then
        tree.lhs(1).dims(1)=prod(1:tree.rhs(1).dims(2))
        tree.lhs(1).dims(2)=tree.rhs(1).dims(2)
    elseif (type(tree.rhs(1).dims(2))==1 & tree.rhs(1).dims(2)==1) & (type(tree.rhs(1).dims(1))==1 & tree.rhs(1).dims(1)<>-1) then
        tree.lhs(1).dims(1)=prod(1:tree.rhs(1).dims(1))
        tree.lhs(1).dims(2)=tree.rhs(1).dims(1)
    else
        tree.lhs(1).dims(1)=-1
        tree.lhs(1).dims(2)=-1
    end

    tree.lhs(1).type=tree.rhs(1).type

endfunction
