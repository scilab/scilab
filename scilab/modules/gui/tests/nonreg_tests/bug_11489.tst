// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 11489 -->
// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11489
//
// <-- Short Description -->
// 'SliderStep' property was ignored for uicontrols.

// 1 - Create a slider without setting the SliderStep property
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

// Click on the slider arrows and check that the value in the edit change with a step of 10
// Click in the slider (arround the knob) and check that the value in the edit change with a step of 100

delete(gcf())

// 2 - Create a slider setting only the first value of the SliderStep property
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
        "SliderStep", 50, ..
        "Callback", "cbSlider");

function cbSlider()
    s = findobj("Tag", "slider");
    e = findobj("Tag", "edit");
    sVal = get(s, "value");
    set(e, "String", string(sVal));
endfunction

// Click on the slider arrows and check that the value in the edit change with a step of 50
// Click in the slider (arround the knob) and check that the value in the edit change with a step of 100

// 3 - Create a slider setting the SliderStep property
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
        "SliderStep", [100 200], ..
        "Callback", "cbSlider");

function cbSlider()
    s = findobj("Tag", "slider");
    e = findobj("Tag", "edit");
    sVal = get(s, "value");
    set(e, "String", string(sVal));
endfunction

// Click on the slider arrows and check that the value in the edit change with a step of 100
// Click in the slider (arround the knob) and check that the value in the edit change with a step of 200

delete(gcf())
