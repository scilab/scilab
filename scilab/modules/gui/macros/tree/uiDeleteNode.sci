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

function myNewTree = uiDeleteNode(tree, position)

    [lhs,rhs]=argn(0);

    //Input arguments checking
    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"), "uiDeleteNode",2));
        return;
    end

    // Check 1st and 2nd inputs : tree & (position or node)
    if rhs == 2 then
        if (typeof(tree) == "uitree") then
            myTree = tree;
            isPosition = %F;
            isNode = %F;
        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d: uitree expected.\n"), "uiDeleteNode",1));
            return;
        end

        if (type(position) == 10) then
            myPosition = position;
            isPosition = %T;

        elseif (typeof(position) == "uitree") then
            myNode = position;
            isNode = %T;
        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d: String or uitree expected.\n"), "uiDeleteNode",2));
            return;
        end
    end

    // Check if the given position exist
    function existPos = existPosition(myTree, myPosition, existPos, curPos)

        if (myPosition == curPos) then
            existPos = %T;
        end

        for index = 3:size(myTree)
            if curPos ~= "root" then
                existPos = existPosition(myTree(index), myPosition, existPos, curPos+"."+string(index-2))
            else
                existPos = existPosition(myTree(index), myPosition, existPos, string(index-2))
            end
        end
    endfunction

    // Find the node at the given position and delete it
    function r = findAndDelete(myTree, atPosition, curpos)

        r = uiCreateTree(myTree);
        for index = 3:size(myTree)

            if curpos ~= "root" then
                localpos = curpos+"."+string(index-2);
            else
                localpos = string(index-2);
            end

            // We don't do the concatenation for the given position
            // What means making a deletion
            if localpos <> atPosition then
                r = uiConcatTree(r,findAndDelete(myTree(index), atPosition, localpos));
            end
        end
    endfunction

    // Deletion with a position
    if isPosition then

        // Check if the given position is valid(exists in the tree)
        existPos = %F;
        existPos = existPosition(myTree, myPosition, existPos, "root");

        if existPos then
            myNewTree = findAndDelete(myTree, myPosition, "root");
        else
            error(msprintf(gettext("%s: Invalid position ''%s''.\n"), "uiDeleteNode",myPosition));
        end
    end

    // Deletion with a node
    if isNode then

        // Check if the given node is valid(exists in the tree OR too many matching nodes)
        r = uiFindNode(myTree, myNode);

        if (size(r) == 1) then

            nodePosList = uiGetNodePosition(myTree, myNode);

            if size(nodePosList) == 1 then
                nodePos = nodePosList(1);

                myNewTree = findAndDelete(myTree, nodePos, "root");

            elseif size(nodePosList) > 1 then
                error(msprintf(gettext("%s:  #%d matching nodes.\n"), "uiDeleteNode",size(nodePosList)));
                return;
            else
                error(msprintf(gettext("%s: Invalid node.\n"), "uiDeleteNode"));
                return;
            end

        elseif (size(r) > 1) then
            error(msprintf(gettext("%s:  #%d matching nodes.\n"), "uiDeleteNode",size(r)));
            return;
        elseif (size(r) == 0) then
            error(msprintf(gettext("%s: No matching node.\n"), "uiDeleteNode"));
            return;
        end
    end

endfunction
