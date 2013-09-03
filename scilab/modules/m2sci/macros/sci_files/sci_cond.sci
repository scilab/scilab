// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Farid BELAHCENE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function tree=sci_cond(tree)
    // M2SCI function
    // Conversion function for Matlab cond()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_cond()

    if rhs == 1 then
        A = getrhs(tree);
        for i=1:size(A.dims)
            if A.dims(i) == -1 | A.dims(i) == 0 then
                tree.name = "mtlb_cond";
            end
        end
    elseif rhs == 2
        tree.name = "mtlb_cond";
    end
    tree.lhs(1).type = Type(Double,Real);
    tree.lhs(1).dims =list(1,1);
endfunction