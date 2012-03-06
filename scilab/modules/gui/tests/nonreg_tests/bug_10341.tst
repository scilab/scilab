// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 10341 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/10341
//
// <-- Short Description -->
// font in help was bold when the font of a text uicontrol was set to bold.

uicontrol("Style", "text", "FontWeight", "bold", "String", "Hello world")
help uigetfile // the displayed font must not be bold
