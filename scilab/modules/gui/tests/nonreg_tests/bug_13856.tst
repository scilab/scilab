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
// <-- Non-regression test for bug 13856 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13856
//
// <-- Short Description -->
// messagebox makes Scilab 5.5 family crashed under Windows.

messagebox("msg", "modal", ["1", "2"]);
messagebox("msg", "messagebox title", "modal");
messagebox("msg", "modal", "question");
messagebox("msg", "messagebox title", "modal", ["1", "2"]);
