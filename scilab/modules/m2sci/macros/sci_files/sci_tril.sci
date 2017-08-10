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

function [tree]=sci_tril(tree)
    // File generated from sci_PROTO7.g: PLEASE DO NOT EDIT !
    // M2SCI function
    // Conversion function for Matlab tril()
    // Input: tree = Matlab funcall tree
    // Output: tree = Scilab equivalent for tree
    // Emulation function: mtlb_tril()

    // L = tril(X,k)
    if rhs==2 then
        [X,k] = getrhs(tree)
        // Convert k to double
        k=convert2double(k)
        tree.rhs=Rhs_tlist(X,k)
        // L = tril(X)
    else
        X = getrhs(tree)
    end

    // Common part
    if or(X.vtype==[String,Unknown]) then
        tree.name="mtlb_tril"
    else
        if X.vtype==Boolean then
            X=Funcall("bool2s",1,Rhs_tlist(X),list(Variable("",X.infer)))
            tree.rhs(1)=X
        end
    end

    if isempty(strindex(tree.name,"triu")) then
        tree.lhs(1).type=X.type
    else
        tree.lhs(1).type=Type(X.vtype,Unknown)
    end
    tree.lhs(1).dims=X.dims

endfunction
