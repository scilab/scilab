// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 2111 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2111
//
// <-- Short Description -->
//    With uicontrol() tt's not possible to use callbacks with more than one Lhs argument.


// Define function:
deff("[out1, out2]=testf(in1, in2)",["out1 = in1"; "out2 = in2"; "disp(""Function TestF"")"]);

// Create figure with pushbutton:
h=figure(1);

// The next line causes the descripted error
uicontrol(h,"callback","[txt,val]=testf(""myString"",37);disp(val,txt);");

// And then click on the button