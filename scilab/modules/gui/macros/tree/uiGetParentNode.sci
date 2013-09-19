// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre Koumar
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function parentNode = uiGetParentNode(tree, node)

    [lhs,rhs]=argn(0);

    //Input arguments checking
    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"), "uiGetParentNode",2));
        return;
    end

    // Check 1st and 2nd inputs : tree & (node or position)
    if rhs == 2 then
        if (typeof(tree) == "uitree") then
            myTree = tree;
            isNode = %F;
            isPosition = %F;
        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d: uitree expected.\n"), "uiGetParentNode",1));
            return;
        end

        if (typeof(node) == "uitree") then
            myNode = node;
            isNode = %T;

        elseif (type(node) == 10) then
            myPosition = node;
            isPosition = %T;

        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d: String or uitree expected.\n"), "uiGetParentNode",2));
            return;
        end
    end


    // Find parent node
    function [parentNode, found]= findParent(myTree, sonNode)

        found = %f;
        parentNode = list();
        // if the son tree exist
        if uiEqualsTree(myTree, sonNode) then
            found = %t;
        end

        if ~found then
            for index = 3:size(myTree)
                [parentNode, found] = findParent(myTree(index), sonNode)
                if parentNode ~= list() then
                    break;
                end
                if found then
                    found = %f;
                    parentNode = uiCreateTree(myTree);
                    break;
                end
            end
        end

    endfunction

    if isNode then
        // First find if the node exist and if we have multiple matching nodes
        result = uiFindNode(myTree, myNode)
    end

    if isPosition then
        // First find if the position exist and if we have multiple matching nodes
        result = uiFindNode(myTree, myPosition)
    end

    if (size(result) == 1) then
        sonNode = result(1);

        [parentNode, found] = findParent(myTree, sonNode)
    else
        error(msprintf(gettext("%s:  #%d matching nodes.\n"), "uiGetParentNode", size(result)));
        return;
    end

endfunction
