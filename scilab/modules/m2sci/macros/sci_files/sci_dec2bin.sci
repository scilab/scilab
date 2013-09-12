// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Farid BELAHCENE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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