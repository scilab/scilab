// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS 
// 
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function tabs = createTab(uiType)
    f = figure("dockable", "off" ,"infobar_visible", "off", "toolbar_visible", "off", "menubar_visible", "off", "default_axes", "off", "layout", "border")
    f.axes_size = [470, 410];

    tabs = uicontrol(f, "style", uiType, "tag" ,"tab", "constraints", createConstraints("border", "center"));

    tab1 = uicontrol(tabs, "style", "frame", "tag", "tab 1", "string", "tab 1", "layout", "border");
    uicontrol(tab1, "string", "button 1");

    tab2 = uicontrol(tabs, "style", "frame", "tag", "tab 2", "string", "tab 2", "layout", "border");
    uicontrol(tab2, "string", "button 2");

    tab3 = uicontrol(tabs, "style", "frame", "tag", "tab 3", "string", "tab 3", "layout", "border");
    uicontrol(tab3, "string", "button 3");
endfunction

//tab
tab = createTab("tab");

assert_checkequal(tab.value, 1);
assert_checkequal(tab.string, "tab 3");
tab.value = 2;
assert_checkequal(tab.value, 2);
assert_checkequal(tab.string, "tab 2");
tab.value = 3;
assert_checkequal(tab.value, 3);
assert_checkequal(tab.string, "tab 1");
tab.value = 1;
assert_checkequal(tab.value, 1);
assert_checkequal(tab.string, "tab 3");

tab.children(1).tag = "newTag";
assert_checkequal(tab.value, 1);
assert_checkequal(tab.string, "newTag");
tab.value = 3;
assert_checkequal(tab.value, 3);
assert_checkequal(tab.string, "tab 1");
tab.value = 1;
assert_checkequal(tab.value, 1);
assert_checkequal(tab.string, "newTag");

delete(gcf());

//layer
layer = createTab("layer");

assert_checkequal(layer.value, 1);
assert_checkequal(layer.string, "tab 3");
layer.value = 2;
assert_checkequal(layer.value, 2);
assert_checkequal(layer.string, "tab 2");
layer.value = 3;
assert_checkequal(layer.value, 3);
assert_checkequal(layer.string, "tab 1");
layer.value = 1;
assert_checkequal(layer.value, 1);
assert_checkequal(layer.string, "tab 3");

layer.children(1).tag = "newTag";
assert_checkequal(layer.value, 1);
assert_checkequal(layer.string, "newTag");

layer.value = 3;
assert_checkequal(layer.value, 3);
assert_checkequal(layer.string, "tab 1");
layer.value = 1;
assert_checkequal(layer.value, 1);
assert_checkequal(layer.string, "newTag");

delete(gcf());
