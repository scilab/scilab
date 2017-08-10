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

function childrenNode = uiGetChildrenNode(tree, node)

    [lhs,rhs]=argn(0);

    //Input arguments checking
    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"), "uiGetChildrenNode",2));
        return;
    end

    // Check 1st and 2nd inputs : tree & (node or position)
    if rhs == 2 then
        if (typeof(tree) == "uitree") then
            myTree = tree;
            isNode = %F;
            isPosition = %F;
        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d: uitree expected.\n"), "uiGetChildrenNode",1));
            return;
        end

        if (typeof(node) == "uitree") then
            myNode = node;
            isNode = %T;

        elseif (type(node) == 10) then
            myPosition = node;
            isPosition = %T;

        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d: String or uitree expected.\n"), "uiGetChildrenNode",2));
            return;
        end
    end

    // Find children nodes
    function childrenNode = findChildren(myTree, childrenNode)

        childrenNode($+1) = myTree;

        for index = 3:size(myTree)
            childrenNode = findChildren(myTree(index), childrenNode)
        end

    endfunction

    if isNode then
        // First find if the node exists and if we have multiple matching nodes
        result = uiFindNode(myTree, myNode)
    end

    if isPosition then
        // First find if the position exists and if we have multiple matching nodes
        result = uiFindNode(myTree, myPosition)
    end

    if (size(result) == 1) then
        parentNode = result(1);

        result2 = list();
        // List containing children nodes
        result2 = findChildren(parentNode, result2);

        childrenNode = list();
        for index = 2:size(result2)
            childrenNode($+1) = result2(index);
        end
    else
        childrenNode = list();
    end

endfunction
