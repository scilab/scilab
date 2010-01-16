// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

//
// <-- Non-regression test for bug 6441 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6441
//
// <-- Short Description -->
// When I create a uimenu with the option 'checked' set, the callback of this menu is never called.

h = scf();

// Remove old menus
delmenu(h.figure_id,gettext("File"));
delmenu(h.figure_id,gettext("Tools"));
delmenu(h.figure_id,gettext("Edition"));
delmenu(h.figure_id,gettext("?"));
toolbar(h.figure_id,"off");

h_file = uimenu(h, "label","&File");
h_file_open = uimenu(h_file,"label","&Open","callback","disp(""!!callback!!"")","checked","off");

// Check that the message "!!callback!!" appears in the console each time you click on the menu File/Open and the menu become checked/unchecked