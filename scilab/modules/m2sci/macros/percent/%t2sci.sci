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

function [tree]=%t2sci(tree)
    // M2SCI function
    // Conversion function for Matlab transpose
    // Input: tree = Matlab operation tree
    // Output: tree = Scilab equivalent for tree
    // Emulation function: mtlb_t()

    A = getoperands(tree)

    tree.out(1).dims=list(A.dims(2),A.dims(1))
    tree.out(1).type=A.type

    // Scilab and Matlab transposition do not work in the same way for strings
    if or(A.vtype==[String,Unknown]) then
        tree=Funcall("mtlb_t",1,Rhs_tlist(A),tree.out)
    end

endfunction

