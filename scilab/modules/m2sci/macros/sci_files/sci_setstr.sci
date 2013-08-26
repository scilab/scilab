// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_setstr(tree)
    // M2SCI function
    // Conversion function for Matlab setstr()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_setstr()

    X = getrhs(tree)

    if X.vtype==String then
        set_infos(msprintf(gettext("In %s %s is a String,\nSo result is set to %s."),expression2code(tree),expression2code(X),expression2code(X)),0)
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
