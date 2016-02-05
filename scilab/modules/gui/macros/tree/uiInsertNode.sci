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

function myNewTree = uiInsertNode(tree, position, node)

    [lhs,rhs]=argn(0);

    //Input arguments checking
    if rhs <> 3 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"), "uiInsertNode",3));
        return;
    end

    // Check 1st, 2nd and 3rd inputs : tree, (position or parent node) & node
    if rhs == 3 then
        if (typeof(tree) == "uitree") then
            myTree = tree;
            isPosition = %F;
            isParentNode = %F;
        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d: uitree expected.\n"), "uiInsertNode",1));
            return;
        end

        if (type(position) == 10) then
            myPosition = position;
            isPosition = %T;

        elseif (typeof(position) == "uitree") then
            myParentNode = position;
            isParentNode = %T;
        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d: String or uitree expected.\n"), "uiInsertNode",2));
            return;
        end

        if (typeof(node) == "uitree") then
            myNode = node;
        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d: uitree expected.\n"), "uiInsertNode",3));
            return;
        end
    end

    // Return the previous position of a node
    function prevPos = previousPosition(myTree, myPosition)

        // Tokenise the position
        TOKENS = [];
        token = strtok(myPosition,".");
        TOKENS = [TOKENS,token];
        while( token <> "" )
            token = strtok(".");
            TOKENS = [TOKENS,token];
        end

        nbElement = size(TOKENS,"*")-1; // because the tokens have the last element as " "

        // Previous position of "1" returns "root"
        if myPosition == "1" then
            prevPos = "root";
        end

        // if we have an integer position (ex: 1, 2, 3 ...)
        if nbElement == 1 & myPosition <> "1" then
            pos = eval(TOKENS(1));
            prevPos = string(pos-1);
        end

        // if we have a complexe position (ex: 1.1, 2.4, 3.8 ...)
        if nbElement > 1 then

            first = eval(TOKENS(nbElement-1));
            second = eval(TOKENS(nbElement));

            if (second == 1) then
                prevPos = TOKENS(1);
                for index = 2:nbElement-1
                    prevPos = prevPos + "." + TOKENS(index);
                end
            elseif (second > 1) then

                prevPos = TOKENS(1);
                for index = 2:nbElement-1
                    prevPos = prevPos + "." + TOKENS(index);
                end

                pos = eval(TOKENS(nbElement));
                prevPos = prevPos + "." + string(pos-1);
            end
        end

        // Previous position of "0" or "root" returns an error
        if myPosition == "0" | myPosition == "root" then
            error(msprintf(gettext("%s: Invalid position ''%s''.\n"), "uiInsertNode",myPosition));
            return;
        end

    endfunction

    // Check if the position exist
    function existPos = existPosition(myTree, myPosition, existPos, curPos)

        if (myPosition == curPos) then
            existPos = %T;
        end

        for index = 3:size(myTree)
            if curPos ~= "root" then
                existPos = existPosition(myTree(index), myPosition, existPos, curPos+"."+string(index-2));
            else
                existPos = existPosition(myTree(index), myPosition, existPos, string(index-2));
            end
        end

    endfunction

    // Find a node and do the insertion with existing position
    function r = findAndInsert(myTree, atPosition, curpos, node)

        r = uiCreateTree(myTree);
        for index = 3:size(myTree)

            if curpos ~= "root" then
                localpos = curpos+"."+string(index-2);
            else
                localpos = string(index-2);
            end

            if localpos == atPosition then
                r = uiConcatTree(r, node);
            end
            r = uiConcatTree(r,findAndInsert(myTree(index), atPosition, localpos, node));
        end

    endfunction

    // Insertion in a node which the position don't exists
    function r = findAndConcat(myTree, atPosition, curpos, node)

        r = uiCreateTree(myTree);
        for index = 3:size(myTree)

            if curpos ~= "root" then
                localpos = curpos+"."+string(index-2);
            else
                localpos = string(index-2);
            end

            r = uiConcatTree(r,findAndConcat(myTree(index), atPosition, localpos, node));
            if localpos == atPosition
                r = uiConcatTree(r, node);
            end
        end

    endfunction

    // Insertion with a position
    if isPosition then

        prevPos = "";
        prevPos = previousPosition(myTree, myPosition);

        existPos = %F;
        existPrevPos = %F;
        existPos = existPosition(myTree, myPosition, existPos, "root");
        existPrevPos = existPosition(myTree, prevPos, existPrevPos, "root");


        if (existPos & existPrevPos) then

            myNewTree = findAndInsert(myTree, myPosition, "root", myNode);

        elseif (existPos == %F & existPrevPos == %T) then

            parentNode = uiGetParentNode(myTree, prevPos);
            parentNode($+1) = myNode;

            myNewTree = findAndConcat(myTree, prevPos, "root", myNode);

        else
            error(msprintf(gettext("%s: Invalid position ''%s''.\n"), "uiInsertNode",myPosition));
            return;
        end

    end

    // Insertion with a parent node
    if isParentNode then

        nodePosList = uiGetNodePosition(myTree, myParentNode);

        if size(nodePosList) == 1 then
            parentNodePos = nodePosList(1);

            myNewTree = findAndConcat(myTree, parentNodePos, "root", myNode);

        elseif size(nodePosList) > 1 then
            error(msprintf(gettext("%s:  #%d matching parent nodes.\n"), "uiInsertNode",size(nodePosList)));
            return;
        else
            error(msprintf(gettext("%s: Invalid parent node.\n"), "uiInsertNode"));
            return;
        end
    end

endfunction
