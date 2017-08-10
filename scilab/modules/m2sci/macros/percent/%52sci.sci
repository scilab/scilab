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

function [tree]=%52sci(tree)
    // M2SCI function
    // Conversion function for Matlab negation
    // Input: tree = Matlab operation tree
    // Output: tree = Scilab equivalent for tree

    A = getoperands(tree)
    if and(A.vtype<>[Double,Boolean]) then
        A = convert2double(A)
        tree.operands=list(A)
    end

    tree.out(1).dims=A.dims
    tree.out(1).type=Type(Boolean,Real)

endfunction
