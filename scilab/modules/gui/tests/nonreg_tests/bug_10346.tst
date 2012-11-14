// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 10346 -->
// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/10346
//
// <-- Short Description -->
// An uicontrol is visible during few millisecs even if its property "Visible" is set to "off"

f=figure();
for i=1:1000
    uicontrol("Visible", "off", "String", "Hello");
end
// Check you never see the uicontrol