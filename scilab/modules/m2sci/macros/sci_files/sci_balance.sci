// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [tree]=sci_balance(tree)
    // M2SCI function
    // Conversion function for Matlab balance()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    A = getrhs(tree)

    // %c_balance and %b_balance are not defined
    A=convert2double(A)
    tree.rhs=Rhs_tlist(A)

    tree.name="balanc"

    // B=balance(A)
    if lhs==1 then
        if tree.lhs(1).name=="ans" then
            B=gettempvar(1)
            T=gettempvar(2)
            m2sci_insert(Equal(list(B,T),tree))
            tree=B
        else
            T=gettempvar(2)
            tree.lhs=list(tree.lhs(1),T)
        end
        tree.lhs(1).dims=A.dims
        tree.lhs(1).type=A.type
        tree.lhs(2).dims=A.dims
        tree.lhs(2).type=Type(Double,Real)
        // [T,B] = balance(A)
    else
        tree.lhs=list(tree.lhs(2),tree.lhs(1))
        tree.lhs(1).dims=A.dims
        tree.lhs(1).type=Type(Double,Real)
        tree.lhs(2).dims=A.dims
        tree.lhs(2).type=A.type
    end

endfunction
