// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 11070 -->
// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11070
//
// <-- Short Description -->
// The "Visible"/"Enable"/"Position" properties of a frame uicontrol did not impact its children.

// 1 - Copy/paste the following lines into Scilab
fig = scf(0);
frameH = uicontrol( ...
        "parent"              , fig,...
        "style"               , "frame",...
        "string"              , "test",...
        "units"               , "pixels",...
        "position"            , [0 0 200 200],...
        "background"          , [1 1 1]*0.8);
editH = uicontrol( ...
        "parent"              , frameH,...
        "style"               , "edit",...
        "string"              , "test",...
        "units"               , "pixels",...
        "position"            , [100 100 60 40],...
        "fontunits"           , "points",...
        "fontsize"            , 24,...
        "background"          , [1 1 1]*0.8);
// Check that the frame and the edit are visible & enabled

// 2 - Copy/paste the following line into Scilab
frameH.enable = "off";
// Check that the frame and the edit are visible & disabled

// 3 - Copy/paste the following line into Scilab
frameH.enable = "on";
// Check that the frame and the edit are visible & enabled

// 4 - Copy/paste the following lines into Scilab
editH.enable = "off";
frameH.enable = "off";
frameH.enable = "on";
// Check that the frame is visible and enable
// Check that the edit is visible & disabled

// 5 - Copy/paste the following lines into Scilab
editH.enable = "on";
// Check that the frame and the edit are visible & enabled

// 6 - Copy/paste the following line into Scilab
frameH.visible = "off";
// Check that the frame and the edit are invisible

// 3 - Copy/paste the following line into Scilab
frameH.visible = "on";
// Check that the frame and the edit are visible & enabled

// 4 - Copy/paste the following lines into Scilab
editH.visible = "off";
frameH.visible = "off";
frameH.visible = "on";
// Check that the frame is visible and enable
// Check that the edit is invisible

// 5 - Copy/paste the following lines into Scilab
editH.visible = "on";
// Check that the frame and the edit are visible & enabled

// 6 - Copy/paste the following lines into Scilab
frameH.position(1) = 200;
// Check that the frame position has changed and that the edit position has been updated