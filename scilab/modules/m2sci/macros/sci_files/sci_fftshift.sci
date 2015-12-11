// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_fftshift(tree)
    // M2SCI function
    // Conversion function for Matlab fftshift()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    if or(tree.rhs(1).vtype==[String,Unknown]) then
        tree.name="mtlb_fftshift"
    end
    if rhs==2 then
        if typeof(tree.rhs(2))=="cste" then
            if tree.rhs(2).value>size(tree.rhs(1).dims) then
                tree.name="mtlb_fftshift"
            end
        else
            tree.name="mtlb_fftshift"
        end
    end
    tree.lhs(1).dims=tree.rhs(1).dims
    tree.lhs(1).type=Type(tree.rhs(1).vtype,tree.rhs(1).property)
endfunction
