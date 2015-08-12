// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_logical(tree)
    // M2SCI function
    // Conversion function for Matlab logical()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_logical()

    X = getrhs(tree)
    // Conversion to double is made to have the same results for strings
    if or(X.vtype==[String,Unknown]) then
        X = convert2double(X)
        tree.rhs=list(X)
    end

    if is_empty(X) then
        set_infos(msprintf(gettext("%s is an empty matrix, so result is set to []."),expression2code(X)),0);
        tree=Cste([])
    elseif not_empty(X) then
        if X.vtype==Boolean then
            if typeof(X)=="operation" then
                X.out(1)=tree.lhs(1)
            elseif typeof(X)=="funcall" then
                X.lhs(1)=tree.lhs(1)
            end

            tree=X
        else
            tree=Operation("<>",list(X,Cste(0)),tree.lhs)

            tree.out(1).dims=X.dims
            tree.out(1).type=Type(Boolean,Real)
        end
    else
        tree.name="mtlb_logical"
        tree.lhs(1).dims=list(Unknown,Unknown)
        tree.lhs(1).type=Type(Boolean,Real)
    end
endfunction
