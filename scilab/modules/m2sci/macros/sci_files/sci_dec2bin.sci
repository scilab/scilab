// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Farid BELAHCENE
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function tree=sci_dec2bin(tree)
    // M2SCI function
    // Conversion function for Matlab dec2bin()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    proddims=1
    for k=tree.rhs(1).dims
        proddims=k*proddims
    end
    tree.lhs(1).dims=list(proddims,1)
    tree.lhs(1).type=Type(String,Real)
endfunction