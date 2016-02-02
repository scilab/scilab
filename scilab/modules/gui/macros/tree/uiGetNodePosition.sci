// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre Koumar
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function position = uiGetNodePosition(tree, node)

    [lhs,rhs]=argn(0);

    //Input arguments checking
    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"), "uiGetNodePosition",2));
        return;
    end

    // Check 1st and 2nd inputs : tree & node
    if rhs == 2 then
        if (typeof(tree) == "uitree") then
            myTree = tree;
        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d: uitree expected.\n"), "uiGetNodePosition",1));
            return;
        end

        if (typeof(node) == "uitree") then
            myNode = node;
        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d: uitree expected.\n"), "uiGetNodePosition",2));
            return;
        end
    end

    // Find node(s) position(s)
    function r = findPos(myTree, myNode, r, curpos)

        if uiEqualsTree(myTree, myNode) then
            r($+1) = curpos;
        end

        for index = 3:size(myTree)
            if curpos ~= "root" then
                r = findPos(myTree(index), myNode, r, curpos+"."+string(index-2))
            else
                r = findPos(myTree(index), myNode, r, string(index-2))
            end
        end

    endfunction

    // List of matching nodes
    r = list();
    position = findPos(myTree, myNode, r, "root");

endfunction
