// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 14578 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14578
//
// <-- Short Description -->
// LaTeX string used for text uicontrol was not updated.

f=figure();
T=uicontrol("parent", f,"style","text","string","$\sqrt{x+1}$","position",[0,200,300,100]);
T.string="$\sin(x+3)$";

