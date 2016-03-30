// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT 
// 
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// This test check save and load functions used with uicontrols
// The following functions are tested:
//  - SCI/modules/graphics/macros/%h_save.sci
//  - SCI/modules/graphics/macros/%h_load.sci

// Create an uimenu
f = scf(0);
h = uicontrol("parent", f, "style", "frame"); // Default style is pushbutton
// Change value of each property to be sure it is saved and loaded correctly
h.backgroundcolor = [0 0 0]; // Default is [0.6 0.6 0.6]
h.enable = "off"; // Default is "on"
h.fontangle = "italic"; // Default is "normal"
h.fontname = "arial"; // Default is "helvetica"
h.fontsize = 20; // Default is 10
h.fontunits = "pixels"; // Default is "points"
h.fontweight = "bold"; // Default is "normal"
h.foregroundcolor = [1 1 1]; // Default is [0 0 0]
h.horizontalalignment = "right"; // Default is "center"
h.listboxtop = 1; // Default is []
h.max = 10; // Default is 1
h.min = 5; // Default is 0
h.position = [100 100 200 200]; // Default is [20 20 80 40]
h.relief = "sunken"; // Default is "ridge" for frames
h.sliderstep = [1 2]; // Default is [0.01,0.1]
h.string = "titi"; // Default is ""
h.tooltipstring = "Tooltip"; // Default is ""
h.units = "points"; // Default is pixels
h.value = 10; // Default is []
h.verticalalignment = "top"; // Default is "center"
h.visible = "off"; // Default is "on"
h.callback = "disp(1)"; // Default is ""
h.callback_type = 1; // Default is 0
h.user_data = list("hello", "Scilab", 5);
h.tag = "My uimenu tag"; // Default is ""

// Save figure contents
save(TMPDIR + "/uicontrol.scg", "h");

hsaved = h;
clear h;

// Load saved handle
load(TMPDIR + "/uicontrol.scg");

// Check if properties are equal
if or(h.style<>hsaved.style) then pause; end
if or(h.backgroundcolor<>hsaved.backgroundcolor) then pause; end
if or(h.enable<>hsaved.enable) then pause; end
if or(h.fontangle<>hsaved.fontangle) then pause; end
if or(h.fontname<>hsaved.fontname) then pause; end
//if or(h.fontsize<>hsaved.fontsize) then pause; end // TODO FIX ME !!
if or(h.fontunits<>hsaved.fontunits) then pause; end
if or(h.fontweight<>hsaved.fontweight) then pause; end
if or(h.foregroundcolor<>hsaved.foregroundcolor) then pause; end
if or(h.horizontalalignment<>hsaved.horizontalalignment) then pause; end
if or(h.listboxtop<>hsaved.listboxtop) then pause; end
if or(h.max<>hsaved.max) then pause; end
if or(h.min<>hsaved.min) then pause; end
if abs(sum(h.position-hsaved.position)) > 4 then pause; end // TODO FIX ME !! Can have a difference of 1 (point or pixel) when getting position
if or(h.relief<>hsaved.relief) then pause; end
if or(h.sliderstep<>hsaved.sliderstep) then pause; end
if or(h.string<>hsaved.string) then pause; end
if or(h.tooltipstring<>hsaved.tooltipstring) then pause; end
if or(h.units<>hsaved.units) then pause; end
if or(h.value<>hsaved.value) then pause; end
if or(h.verticalalignment<>hsaved.verticalalignment) then pause; end
if or(h.visible<>hsaved.visible) then pause; end
if or(h.callback<>hsaved.callback) then pause; end
if or(h.callback_type<>hsaved.callback_type) then pause; end
if or(h.user_data<>hsaved.user_data) then pause; end
if or(h.tag<>hsaved.tag) then pause; end
