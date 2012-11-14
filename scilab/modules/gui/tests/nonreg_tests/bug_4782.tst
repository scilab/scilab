// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
// Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

//
// <-- Non-regression test for bug 4782 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4782
//
// <-- Short Description -->
//    Can not save/display uicontextmenu


h = uicontextmenu();
h1 = uimenu("Parent", h, "label", "foo1", "Callback", "disp hello");
h2 = uimenu("Parent", h, "label", "foo2", "Callback", "disp coucou");

// Before this bug fix, uicontextmenus could not be displayed (crash)
disp(h)

// Before this bug fix, uicontextmenus were not be saved
save(TMPDIR + filesep() + "bug_4782.tmp", "h");
child1label = get(h.children(1),"label");
child2label = get(h.children(2),"label");
clear h
load(TMPDIR + filesep() + "bug_4782.tmp");
deletefile(TMPDIR + filesep() + "bug_4782.tmp");

disp(h)
// Check that children are saved and loaded in the same order
if size(h.children,"*")<>2 then pause;end
if get(h.children(1),"label")<>child1label then pause;end
if get(h.children(1),"label")<>"foo2" then pause;end
if get(h.children(2),"label")<>child2label then pause;end
if get(h.children(2),"label")<>"foo1" then pause;end

clear h hsave h1 h2 child1label child2label

// Same test with uimenus (problems with children save/load before this fix)
h = uimenu("Parent", gcf(), "Label", "parentuimenu");
h1 = uimenu("Parent", h, "label", "uimenufoo1", "Callback", "disp hello");
h2 = uimenu("Parent", h, "label", "uimenufoo2", "Callback", "disp coucou");

// Before this bug fix, uimenus children were not be saved
save(TMPDIR + filesep() + "bug_4782.tmp", "h");
hsave = h;
clear h
load(TMPDIR + filesep() + "bug_4782.tmp");
deletefile(TMPDIR + filesep() + "bug_4782.tmp");

// Check that children are saved and loaded in the same order
if size(h.children,"*")<>2 then pause;end
if get(h.children(1),"label")<>get(hsave.children(1),"label") then pause;end
if get(h.children(1),"label")<>"uimenufoo2" then pause;end
if get(h.children(2),"label")<>get(hsave.children(2),"label") then pause;end
if get(h.children(2),"label")<>"uimenufoo1" then pause;end

clear h hsave h1 h2
