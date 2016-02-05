// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 13185 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13185
//
// <-- Short Description -->
//  When the "checked" option of an uimenu was set to "on" for
//  the first time, the display order of others uimenus was changed.

function f_menu1()
    obj = findobj("tag", "menu1");
    if obj.checked == "on" then
        obj.checked = "off";
    else
        obj.checked = "on";
    end
endfunction

function f_menu2()
    obj = findobj("tag", "menu2");
    if obj.checked == "on" then
        obj.checked = "off";
    else
        obj.checked = "on";
    end
endfunction

function f_menu3()
    obj = findobj("tag", "menu3");
    if obj.checked == "on" then
        obj.checked = "off";
    else
        obj.checked = "on";
    end
endfunction


f = gcf()
main_menu = uimenu(f, "label", "Test")
menu_1 = uimenu(main_menu, "label", "menu1", "callback", "f_menu1()", "tag", "menu1")
menu_2 = uimenu(main_menu, "label", "menu2", "callback", "f_menu2()", "tag", "menu2")
menu_3 = uimenu(main_menu, "label", "menu3", "callback", "f_menu3()", "tag", "menu3")

// First, you click on the first menu: "menu1". The menus list must not change.
// Idem for "menu2" and "menu3"
