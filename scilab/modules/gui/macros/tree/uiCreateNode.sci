// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre Koumar
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function myNode = uiCreateNode(label, icon, callback)

    [lhs,rhs]=argn(0);

    //Input arguments checking
    if rhs <= 0 | rhs > 3 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "uiCreateNode",1,3));
        return;
    end

    // Check 1st input : label
    if rhs >= 1 then
        if (type(label) == 10) then
            myLabel = label;
            myIcon = "";
            myCallback = "";
        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"), "uiCreateNode",1));
            return;
        end
        // Check 2nd input : icon
        if rhs >= 2 then
            if (type(icon) == 10) then
                myIcon = icon;
                myCallback = "";
            else
                error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"), "uiCreateNode",2));
                return;
            end
            // Check 3rd input : callback
            if rhs == 3 then
                if (type(callback) == 10) then
                    myCallback = callback;
                else
                    error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"), "uiCreateNode",3));
                    return;
                end
            end

        end

    end

    // Creation of the node
    node = struct("label",myLabel,"icon",myIcon,"callback",myCallback)
    myNode = tlist("uitree", node)

endfunction
