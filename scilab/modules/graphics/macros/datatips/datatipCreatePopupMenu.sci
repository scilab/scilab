// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
