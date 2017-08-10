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

function nodeList = uiFindNode(tree, node, value)

    [lhs,rhs]=argn(0);

    //Input arguments checking
    if rhs < 2 | rhs > 3 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"), "uiFindNode",2,3));
        return;
    end

    // Check 1st and 2nd inputs : tree & (node or property)
    if rhs >= 2 then
        if (typeof(tree) == "uitree") then
            myTree = tree;
            isNode = %F;
            isPosition = %F;
            isProperty = %F;
        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d: uitree expected.\n"), "uiFindNode",1));
            return;
        end

        if (typeof(node) == "uitree") then
            myNode = node;
            isNode = %T;

        elseif (type(node) == 10) then
            if rhs == 2
                myPosition = node;
                isPosition = %T;
            end
        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d: String or uitree expected.\n"), "uiFindNode",2));
            return;
        end

        // Check 3rd input : property's value
        if rhs == 3 then
            if (type(node) == 10 & type(value) == 10) then
                if (node == "label" | node == "icon" | node == "callback") then
                    myProperty = node;
                    myValue = value;
                    isProperty = %T;
                else
                    error(msprintf(gettext("%s: Wrong type for input argument #%d: must be ''label'', ''icon'' or ''callback''.\n"), "uiFindNode",2));
                    return;
                end
            else
                error(msprintf(gettext("%s: Wrong type for input argument #%d or #%d: String expected.\n"), "uiFindNode",2,3));
                return;
            end
        end
    end

    // Find matching node(s)
    function r = internalFindNode(myTree, myNode, r)

        if uiEqualsTree(myTree, myNode) then
            r($+1) = myTree;
        end

        for index = 3:size(myTree)
            r = internalFindNode(myTree(index), myNode, r)
        end

    endfunction

    // Finding node with a given position
    function r = findPos(myTree, myPosition, r, curpos)

        if myPosition == curpos then
            r($+1) = myTree;
        end

        for index = 3:size(myTree)
            if curpos ~= "root" then
                r = findPos(myTree(index), myPosition, r, curpos+"."+string(index-2))
            else
                r = findPos(myTree(index), myPosition, r, string(index-2))
            end
        end

    endfunction

    // Finding node(s) with a given property
    function r = findProperty(myTree, myProperty, myValue, r)

        if myTree(2)(myProperty) == myValue then
            r($+1) = myTree;
        end

        for index = 3:size(myTree)
            r = findProperty(myTree(index), myProperty, myValue, r)
        end

    endfunction

    // List containing result
    r = list();

    // Find with a node
    if isNode then
        // List of matching nodes
        nodeList = internalFindNode(myTree, myNode, r);
        if (size(nodeList) > 1) then
            warning(msprintf(gettext("%s:  #%d matching nodes.\n"), "uiFindNode",size(nodeList)));
            return;
        elseif (size(nodeList) == 0) then
            warning(msprintf(gettext("%s: No results found.\n"), "uiFindNode"));
            return;
        end
    end

    // Find with a position
    if isPosition then
        // List of matching nodes
        nodeList = findPos(myTree, myPosition, r, "root");
        if size(nodeList) == 0 then
            error(msprintf(gettext("%s: Invalid position ''%s''.\n"), "uiFindNode",myPosition));
            return;
        end
    end

    // Find with a property
    if isProperty then
        // List of matching nodes
        nodeList = findProperty(myTree, myProperty, myValue, r)
        if size(nodeList) == 0 then
            warning(msprintf(gettext("%s: No results found for property ''%s'' and value ''%s''.\n"), "uiFindNode",myProperty, myValue));
            return;
        else
            if (size(nodeList) > 1) then
                warning(msprintf(gettext("%s:  #%d matching nodes.\n"), "uiFindNode",size(nodeList)));
                return;
            end
        end
    end

endfunction
