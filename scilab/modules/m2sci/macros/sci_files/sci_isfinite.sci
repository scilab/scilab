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

function tree = sci_isfinite(tree)
    // M2SCI function
    // Conversion function for Matlab isfinite()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    // %c_isfinite and %b_isfinite are not defined in Scilab
    A = getrhs(tree)
    if or(A.vtype==[String Boolean Unknown])
        A = convert2double(A)
        tree.rhs = Rhs_tlist(A)
    end

    tree = Funcall("abs",1, list(A), tree.lhs)
    tree = Operation("<", list(tree,Cste(%inf)), tree.lhs)

    if A.vtype==Sparse | A.property==Sparse
        prop = Sparse
    else
        prop = Boolean
    end

    tree.out(1).dims = A.dims
    tree.out(1).type = Type(Boolean,prop)
endfunction
