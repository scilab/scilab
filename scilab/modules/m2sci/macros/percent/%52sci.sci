// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
