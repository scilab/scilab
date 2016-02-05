// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 2565 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2565
//
// <-- Short Description -->
//    Sscalar uicontrol('listbox') string entries are tcl-substituted, vector string
//    entries not (i.e., dollars, brackets, backslashes are taken literally, etc.)

f = figure(1);
h = uicontrol(f,"style","listbox",..
    "position",[10 10 150 150],..
    "string",["\Q\q"]);

if get(h,"string") <> "\Q\q" then pause; end

set(h,"string",["\Q","\q"]) 

if or(get(h,"string") <> ["\Q","\q"]) then pause; end


