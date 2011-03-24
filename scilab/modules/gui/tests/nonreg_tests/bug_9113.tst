*// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 9113 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9113
//
// <-- Short Description -->
// setlookandfeel could lead to an exception

a=getinstalledlookandfeels();for i=1:100, setlookandfeel(a(modulo(i,3) + 1));end
