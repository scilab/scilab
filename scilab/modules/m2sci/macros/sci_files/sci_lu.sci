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

function [tree]=sci_lu(tree)
    // M2SCI function
    // Conversion function for Matlab lu()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    // lu(X)
    if rhs==1 then
        X = getrhs(tree)
        X = convert2double(X)
        tree.rhs=Rhs_tlist(X)
        // Y = lu(X)
        if lhs==1 then
            if tree.lhs(1).name=="ans" then
                no_equiv(expression2code(tree))
            else
                no_equiv(tree.lhs(1).name+" = "+expression2code(tree))
            end
            // [L,U] = lu(X)
        elseif lhs==2 then
            tree.lhs(1).dims=list(Unknown,Unknown)
            tree.lhs(1).type=Type(X.vtype,Real)
            tree.lhs(2).dims=list(Unknown,Unknown)
            tree.lhs(2).type=X.type
            // [L,U,P] = lu(X)
        elseif lhs==3 then
            tree.lhs(1).dims=list(Unknown,Unknown)
            tree.lhs(1).type=Type(X.vtype,Real)
            tree.lhs(2).dims=list(Unknown,Unknown)
            tree.lhs(2).type=X.type
            tree.lhs(3).dims=list(Unknown,Unknown)
            tree.lhs(3).type=Type(X.vtype,Real)
            // [L,U,P,Q] = lu(X)
        else
            no_equiv("["+tree.lhs(1).name+","+tree.lhs(2).name+","+tree.lhs(3).name+"] = "+expression2code(tree))
        end
        // lu(X,thresh)
    else
        no_equiv(expression2code(tree))
    end
endfunction
