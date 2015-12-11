// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=%02sci(tree)
    // M2SCI function
    // Conversion function for Matlab dot transpose
    // Input: tree = Matlab operation tree
    // Output: tree = Scilab equivalent for tree
    // Emulation function: mtlb_0()

    A = getoperands(tree)

    // Scilab and Matlab transposition do not work in the same way for strings
    if or(A.vtype==[String,Unknown]) then
        tree=Funcall("mtlb_0",1,Rhs_tlist(A),tree.out)
        tree.lhs(1).dims=list(A.dims(2),A.dims(1))
        tree.lhs(1).type=A.type
    else
        if A.vtype==Boolean then
            tree.operands=list(Funcall("bool2s",1,list(A),list()))
        end
        tree.out(1).dims=list(A.dims(2),A.dims(1))
        tree.out(1).type=A.type
    end

endfunction

