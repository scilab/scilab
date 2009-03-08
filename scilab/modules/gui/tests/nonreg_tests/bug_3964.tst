// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

//
// <-- Non-regression test for bug 3964 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3964
//
// <-- Short Description -->
// uigetfile() crashes Scilab if the description of the mask is specified.

// try all lines

uigetfile(["*.bin","BIN files"],"SCI/modules/gui/macros/") 

uigetfile(["*.bin";"*.sce";"*.cos*"]) 

uigetfile(["*.sci";"*.bin"],"SCI/modules/gui/macros/")

uigetfile(["*.sc*";"*.bin"],"SCI/modules/gui/macros/")

uigetfile(["*.sce";"*.bin"],"SCI/modules/gui/macros/", "Choose a file name",%t)

uigetfile(["*.sce";"*.bin"],"SCI/modules/gui/macros/", "Choose a file name",%f)
