// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Allan CORNET
// Copyright (C) 2007 - INRIA - Vincent COUVERT Java version
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function addmenu(varargin)
    // Add menus in Scilab Windows
    // See comments in the code for different implementations

    if getscilabmode() == "NWNI" then
        error(msprintf(_("%s: can not add a menu in this mode: %s.\n"), "addmenu", getscilabmode()));
    end

    rhs=argn(2);

    select rhs
    case 1
        if type(varargin(1))==10
            // addmenu(menu_label)
            // Create a menu in Scilab main Window
            // The callback is the variable named menu_label

            // Error message in not in standard mode (we need a figure index)
            if getscilabmode() <> "STD" then
                error(msprintf(_("%s: can not add a menu in console in this mode: %s.\n"), "addmenu", getscilabmode()));
            end

            if size(varargin(1),"*") <> 1
                error(msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"),"addmenu", 1));
            end

            if getos()=="Darwin" then
                // MACOSX version add a submenu with same name because parent menu can not have a callback
                addMenuSubMenus(0, varargin(1), varargin(1));
            else
                addSingleMenu(0, varargin(1));
            end

        else
            error(msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"),"addmenu", 1));
        end
    case 2
        if type(varargin(1))==10 & type(varargin(2))==10
            // addmenu(menu_label, submenus_labels)
            // Create a menu in Scilab main Window with submenus
            // The callbacks are the items of variable named menu_label

            // Error message in not in standard mode
            if getscilabmode() <> "STD" then
                error(msprintf(_("%s: can not add a menu in console in this mode: %s.\n"), "addmenu", getscilabmode()));
            end

            if size(varargin(1),"*") <> 1
                error(msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"),"addmenu", 1));
            end

            // No verification for submenus labels

            addMenuSubMenus(0, varargin(1), varargin(2));

        elseif type(varargin(1))==10 & type(varargin(2))==15
            // addmenu(menu_label, action_in_a_list)
            // Create a menu in Scilab main Window with submenus
            // The callbacks are the items of variable named menu_label

            // Error message in not in standard mode
            if getscilabmode() <> "STD" then
                error(msprintf(_("%s: can not add a menu in console in this mode: %s.\n"), "addmenu", getscilabmode()));
            end

            if size(varargin(1),"*") <> 1
                error(msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"),"addmenu",1));
            end

            if size(varargin(2)) <> 2
                error(msprintf(_("%s: Wrong type for input argument #%d: A two-item list expected.\n"),"addmenu",2));
            end

            if getos()=="Darwin" then
                // MACOSX version add a submenu with same name because parent menu can not have a callback
                addMenuSubMenusCallback(0, varargin(1), varargin(1), varargin(2));
            else
                addSingleMenuCallback(0, varargin(1), varargin(2));
            end

        elseif type(varargin(1))==1 & type(varargin(2))==10
            // addmenu(graphic_window_id,menu_label)
            // Create a menu in Scilab main Window
            // The callback is the variable named menu_label

            if size(varargin(1),"*") <> 1
                error(msprintf(_("%s: Wrong type for input argument #%d: A real expected.\n"),"addmenu", 1));
            end

            if size(varargin(2),"*") <> 1
                error(msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"),"addmenu", 2));
            end

            if getos()=="Darwin" then
                // MACOSX version add a submenu with same name because parent menu can not have a callback
                addMenuSubMenus(scf(varargin(1)), varargin(2), varargin(2));
            else
                addSingleMenu(scf(varargin(1)), varargin(2))
            end

        else
            error(msprintf(_("%s: Wrong input arguments: %s or %s expected.\n"),"addmenu","(menu_label, action_in_a_list)","(graphic_window_id,menu_label)"));
        end
    case 3
        if type(varargin(1))==10 & type(varargin(2))==10 & type(varargin(3))==15
            // addmenu(menu_label, submenus_labels, actions_in_a_list)
            // Create a menu in Scilab main Window with submenus
            // The callbacks are given in the third input argument

            // Error message in not in standard mode
            if getscilabmode() <> "STD" then
                error(msprintf(_("%s: can not add a menu in console in this mode: %s.\n"), "addmenu", getscilabmode()));
            end

            if size(varargin(1),"*") <> 1
                error(msprintf(_("%s: Wrong type for input argument #%d: A real expected.\n"),"addmenu",1));
            end

            // No verification for submenus labels size

            if size(varargin(3)) <> 2
                error(msprintf(_("%s: Wrong type for input argument #%d: A two-item list expected.\n"),"addmenu",3));
            end

            addMenuSubMenusCallback(0, varargin(1), varargin(2), varargin(3));

        elseif type(varargin(1))==1 & type(varargin(2))==10 & type(varargin(3))==10
            // addmenu(graphic_window_id, menu_label, submenus_labels)
            // Create a menu in Scilab main Window with submenus
            // The callbacks are the items of variable named menu_label

            if size(varargin(1),"*") <> 1
                error(msprintf(_("%s: Wrong type for input argument #%d: A real expected.\n"),"addmenu",1));
            end

            if size(varargin(2),"*") <> 1
                error(msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"),"addmenu",2));
            end

            // No verification for submenus labels

            addMenuSubMenus(scf(varargin(1)), varargin(2), varargin(3));

        elseif type(varargin(1))==1 & type(varargin(2))==10 & type(varargin(3))==15
            // addmenu(graphic_window_id, menu_label, action_in_a_list)
            // Create a menu in Scilab main Window with submenus
            // The callbacks are the items of variable named menu_label

            if size(varargin(1),"*") <> 1
                error(msprintf(_("%s: Wrong type for input argument #%d: A real expected.\n"),"addmenu", 1));
            end

            if size(varargin(2),"*") <> 1
                error(msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"),"addmenu",2));
            end

            if size(varargin(3)) <> 2
                error(msprintf(_("%s: Wrong type for input argument #%d: A two-item list expected.\n"),"addmenu",3));
            end

            if getos()=="Darwin" then
                addMenuSubMenusCallback(scf(varargin(1)), varargin(2), varargin(2), varargin(3));
            else
                addSingleMenuCallback(scf(varargin(1)), varargin(2), varargin(3));
            end

        else
            error(msprintf(_("%s: Wrong input arguments: %s, %s or %s expected.\n"),"addmenu","(menu_label, submenus_labels, actions_in_a_list)","(graphic_window_id, menu_label, submenus_labels)","(graphic_window_id, menu_label, action_in_a_list)"));
        end
    case 4
        // addmenu(graphic_window_id, menu_label, submenus_labels, action_in_a_list)
        // Create a menu in a Scilab graphic Window with submenus
        // The callbacks are given in a list

        if size(varargin(1),"*") <> 1
            error(msprintf(_("%s: Wrong type for input argument #%d: A real expected.\n"),"addmenu",1));
        end

        if size(varargin(2),"*") <> 1
            error(msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"),"addmenu",2));
        end

        // No verification for submenus labels

        if size(varargin(4)) <> 2
            error(msprintf(_("%s: Wrong type for input argument #%d: A two-item list expected.\n"),"addmenu",4));
        end

        addMenuSubMenusCallback(scf(varargin(1)), varargin(2), varargin(3), varargin(4));

    else
        error(msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"),"addmenu", 1,4));
    end
endfunction
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Add a menu with no callback given by the user
//------------------------------------------------------------------------------
function addSingleMenu(fig, menulabel)
    h = uimenu("parent", fig, "label", menulabel);
    if type(fig)==1
        set(h, "callback", list(4, "execstr("+menulabel+"(1))"));
    else
        set(h, "callback", list(4, "execstr("+menulabel+"_"+string(get(fig,"figure_id"))+"(1))"));
    end
endfunction
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Add a menu with callback given by the user
//------------------------------------------------------------------------------
function addSingleMenuCallback(fig, menulabel, callback)

    [callbackStr, callbackType] = getCallbackProperties(callback);

    h = uimenu("parent", fig, "label", menulabel);

    if callbackType == 0 then
        if type(fig)==1
            set(h, "callback", list(4, "execstr("+callbackStr+"(1))"));
        else
            set(h, "callback", list(4, "execstr("+callbackStr+"_"+string(get(fig,"figure_id"))+"(1))"));
        end
    elseif callbackType == 1 then
        if type(fig)==1
            cb = "call(""" + callbackStr + """,""" + ..
            menulabel + """,1,""c""," + ..
            "-1,2,""i""," + ..
            "1,3,""i"",""out"",[1,1],2,""i"")"; // Window number is returned
        else
            cb = "call(""" + callbackStr + """,""" + ..
            menulabel + """,1,""c""," + ..
            string(get(fig,"figure_id"))+ ",2,""i""," +..
            "1,3,""i""," + ..
            """out"",[1,1],2,""i"")"; // Window number is returned
        end
        set(h, "callback", list(4, "execstr("+sci2exp(cb)+")"));
    elseif callbackType == 2 then
        if type(fig)==1
            set(h, "callback", list(4, callbackStr+"(1)"));
        else
            set(h, "callback", list(4, callbackStr+"(1,"+string(get(fig,"figure_id"))+")"));
        end
    end

endfunction
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Add a menu and its submenus with no callback given by the user
//------------------------------------------------------------------------------
function addMenuSubMenus(fig, menulabel, submenuslabels)

    h0 = uimenu("parent", fig, "label", menulabel);

    for K=1:size(submenuslabels,"*")
        if type(fig)==1
            uimenu("parent", h0, "label", submenuslabels(K), "callback", list(4, "execstr("+menulabel+"("+string(K)+"))"));
        else
            uimenu("parent", h0, "label", submenuslabels(K), "callback", list(4, "execstr("+menulabel+"_"+string(get(fig,"figure_id"))+"("+string(K)+"))"));
        end
    end

endfunction
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Add a menu and its submenus with callback given by the user
//------------------------------------------------------------------------------
function addMenuSubMenusCallback(fig, menulabel, submenuslabels, callback)

    [callbackStr, callbackType] = getCallbackProperties(callback);

    h0 = uimenu("parent",fig,"label",menulabel);

    for k=1:size(submenuslabels,"*")
        h = uimenu("parent", h0, "label", submenuslabels(k));

        if callbackType == 0 then
            if type(fig)==1
                set(h, "callback", list(4, "execstr("+callbackStr+"("+string(k)+"))"));
            else
                set(h, "callback", list(4, "execstr("+callbackStr+"_"+string(get(fig,"figure_id"))+"("+string(k)+"))"));
            end
        elseif callbackType == 1 then
            if type(fig)==1
                cb = "call(""" + callbackStr + """,""" + ..
                menulabel + """,1,""c""," + ..
                "-1,2,""i""," + ..
                string(k-1) +",3,""i"",""out"",[1,1],2,""i"")"; // Window number is returned
            else
                cb = "call(""" + callbackStr + """,""" + ..
                menulabel + """,1,""c""," + ..
                string(get(fig,"figure_id")) + ",2,""i""," + ..
                string(k-1)+ ",3,""i""," + ..
                """out"",[1,1],2,""i"")"; // Window number is returned
            end
            set(h, "callback", list(4, "execstr("+sci2exp(cb)+")"));
        elseif callbackType == 2 then
            if type(fig)==1
                set(h, "callback", list(4, callbackStr+"("+string(k)+")"));
            else
                set(h, "callback", list(4, callbackStr+"("+string(k)+","+string(get(fig,"figure_id"))+")"));
            end
        end
    end
endfunction
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Manages callback given as list and check parameters
//------------------------------------------------------------------------------
function [callbackStr, callbackType] = getCallbackProperties(callback)
    callbackType = callback(1);
    if type(callbackType)<>1 | size(callbackType,"*")<>1
        error(msprintf(_("%s: Wrong value for ''%s'' property: %d, %d or %d expected.\n"), "addmenu", "CallbackType", 0, 1, 2));
    end

    callbackStr = callback(2);
    if size(callbackStr,"*")<>1
        error(msprintf(_("%s: Wrong type for ''%s'' property: string expected.\n"), "addmenu", "Callback"));
    end
endfunction
//------------------------------------------------------------------------------
