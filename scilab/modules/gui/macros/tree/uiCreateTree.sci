// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre Koumar
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function myTree = uiCreateTree(parentNode, varargin)

    [lhs,rhs]=argn(0);

    //Input arguments checking
    if rhs == 0 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "uiCreateTree",1));
        return;
    end

    if rhs >=1 then
        // Check if parentNode is a Tree
        if (typeof(parentNode) == "uitree") then
            myTree = tlist("uitree", parentNode(2));
            // Add subTrees into myTree
            for subTreesIndex = 1:size(varargin)
                if (typeof(varargin(subTreesIndex)) == "uitree") then
                    myTree(subTreesIndex + 2) = varargin(subTreesIndex)
                else
                    error(msprintf(gettext("%s: Wrong type for element %d of input argument #%d: uitree expected.\n"), "uiCreateTree", subTreesIndex, 2));
                    return;
                end
            end
        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d: uitree expected.\n"), "uiCreateTree",1));
            return;
        end
    end

endfunction
