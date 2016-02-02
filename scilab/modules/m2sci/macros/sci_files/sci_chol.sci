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

function [tree]=sci_chol(tree)
    // M2SCI function
    // Conversion function for Matlab chol()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    X = getrhs(tree)

    if X.vtype==Sparse then
        tree.name="spchol"
    else
        tree.name="chol"
    end

    // %c_chol and %b_chol are not defined
    X=convert2double(X)
    tree.rhs=Rhs_tlist(X)

    // R = chol(X)
    if lhs==1 then
        tree.lhs(1).dims=X.dims
        tree.lhs(1).type=X.type
        // [R,p] = chol(X)
    else
        no_equiv(gettext("Call to chol() with 2 outputs."))
    end

endfunction
