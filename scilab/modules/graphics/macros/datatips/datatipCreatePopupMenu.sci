// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// Create a popupmenu and waits for a user input
function h = datatipCreatePopupMenu(listOfMenus)
    //datatip utility function
    // Create a contextmenu
    h = uicontextmenu();

    // Create its items
    createPopupItems(h, listOfMenus);

    // Display and wait for a user answer
    Cmenu = uiwait(h);

endfunction

function createPopupItems(parent, listOfMenus)

    for menu=listOfMenus
        if type(menu) == 10 then
            if size(menu,"*")==1 then
                h1 = uimenu("Parent", parent, "label", menu);
            else
                h1 = uimenu("Parent", parent, "label", menu(1), "callback", ...
                menu(2));
            end
        else
            if length(menu) >= 2
                h1 = uimenu("Parent", parent, "label", menu(1));
                menu(1)=null();
                createPopupItems(h1, menu);
            else
                error(gettext("createPopupItems: found an empty cascading menu."));
            end
        end
    end
endfunction
