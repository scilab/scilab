// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 8134 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8134
//
// <-- Short Description -->
// The font attributes (FontName, FontSize, FontWeight) are no more taken into account for "text" uicontrols.

h = uicontrol("Style", "text", ..
    "String", "Bug 8134", ..
    "Position", [100 100 300 200]);

// Change the font size and check it is taken into account
h.fontname = "Times";
// Change the font size and check it is taken into account
h.fontsize = 20;
// Change the font angle and check it is taken into account
h.fontangle = "italic";
// Change the font weight and check it is taken into account
h.fontweight = "bold";

