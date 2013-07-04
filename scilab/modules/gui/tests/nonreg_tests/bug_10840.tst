// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 10840 -->
// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/10840
//
// <-- Short Description -->
// Keyboard arrows were disabled on 'slider' uicontrols.

f = gcf();
editH = uicontrol("Parent", f, ..
        "Style", "edit", ..
        "Position", [60 36 50 16], ..
        "String", "500", ..
        "Backgroundcolor", [1 1 1], ..
        "Tag", "edit");

sliderH = uicontrol("Parent", f, ..
        "Style", "slider", ..
        "Position", [10 8 200 20], ..
        "Value", 500, ..
        "Min", 0, ..
        "Max", 1000, ..
        "Tag", "slider", ..
        "Callback", "cbSlider");

function cbSlider()
    s = findobj("Tag", "slider");
    e = findobj("Tag", "edit");
    sVal = get(s, "value");
    set(e, "String", string(sVal));
endfunction

// Modify the slider values using the up/down/top/left keys and check that the value in the edit changes with a step of 10
// Modify the slider values using the Page Up/Page Down keys and check that the value in the edit change with a step of 100

delete(gcf())

