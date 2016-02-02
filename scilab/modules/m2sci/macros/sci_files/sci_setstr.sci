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

function [tree]=sci_setstr(tree)
    // M2SCI function
    // Conversion function for Matlab setstr()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_setstr()

    X = getrhs(tree)

    if X.vtype==String then
        set_infos(msprintf(gettext("In %s %s is a String,\nSo result is set to %s."), strcat(expression2code(tree), ""), strcat(expression2code(X), ""), strcat(expression2code(X), "")),0)
        // Because of String concatenation
        if typeof(X)=="operation" then
            X.out(1)=tree.lhs(1)
        elseif typeof(X)=="funcall" then
            X.lhs(1)=tree.lhs(1)
        end
        tree=X
    elseif X.vtype==Unknown then
        tree.name="mtlb_setstr"
        tree.lhs(1).dims=X.dims
        tree.lhs(1).type=Type(String,Real)
    else
        tree.name="ascii"
        if X.dims(1)==1 | is_a_scalar(X) then // Row vector or scalar
            tree.lhs(1).dims=X.dims
            tree.lhs(1).type=Type(String,Real)
        else
            tree.name="mtlb_setstr"
            tree.lhs(1).dims=X.dims
            tree.lhs(1).type=Type(String,Real)
        end
    end
endfunction
