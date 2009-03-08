// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 562 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=562
//
// <-- Short Description -->
//    Scilab crashes 99% of the time when the content of a 
//    graphics window, that was put onto the clipboard, is pasted 
//    e. g. into a  Microsoft Word document.
//    This was already described in Bug Report 12044200362417155, 
//    but the answer to that report concentrates only on GIFs. 
//    The more severe problem is the crash of Scilab, not the 
//    graphic file format!
//    Since we use Scilab heavily for measurement, we need to 
//    document test results properly. The best way is to include 
//    Scilab figures into text documents. Without the copy-past 
//    cycle this is not working. It would help us a lot if this 
//    bug was fixed!


plot2d();
mprintf("In the graphic window''s menu: Copy to Clipboard\n");
mprintf("Paste the clipboard into an arbitrary application: Scilab\n");
mprintf("crashes in the background, nothing is pasted\n");
