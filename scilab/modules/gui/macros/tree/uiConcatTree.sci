// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre Koumar
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function myNewTree = uiConcatTree(tree1, tree2)

    [lhs,rhs]=argn(0);


    //Input arguments checking
    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"), "uiConcatTree",2));
        return;
    end

    // Check 1st and 2nd inputs : tree1 & tree2
    if rhs == 2 then
        if (typeof(tree1) == "uitree") then
            myTree1 = tree1;
        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d: uitree expected.\n"), "uiConcatTree",1));
            return;
        end

        if (typeof(tree2) == "uitree") then
            myTree2 = tree2;
        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d: uitree expected.\n"), "uiConcatTree",2));
            return;
        end
    end

    tree1($+1) = tree2;
    myNewTree = tree1;

endfunction
