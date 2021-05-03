// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function tree = sci_full(tree)
    // M2SCI converter for Matlab full()

    S = getrhs(tree)

    tree.lhs(1).dims = S.dims
    tree.lhs(1).type = S.type

    if S.vtype==Sparse then
        tree.lhs(1).type = Type(Double,S.property)

    elseif S.property==Sparse  // Boolean Sparse
        tree.lhs(1).type = Type(Boolean, Boolean)

    end

endfunction
