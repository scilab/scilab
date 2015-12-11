// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
