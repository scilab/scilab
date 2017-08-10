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
