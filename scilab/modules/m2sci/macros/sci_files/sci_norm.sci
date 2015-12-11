// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_norm(tree)
    // M2SCI function
    // Conversion function for Matlab norm()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_norm()

    // n = norm(A)
    if rhs==1 then
        A = getrhs(tree)
        A = convert2double(A)
        tree.rhs=Rhs_tlist(A)
        // n = norm(A,p)
    else
        [A,p] = getrhs(tree)
        A = convert2double(A)
        tree.rhs=Rhs_tlist(A,p)
    end

    tree.lhs(1).dims=list(1,1)
    tree.lhs(1).type=Type(Double,Real)

endfunction
