// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Scilab Enterprises - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 10081 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10081
//
// <-- Short Description -->
// Xcos palette is not cleared on Scilab restart after loading an external
// palette.

xcos();

pal = xcosPal("my palette");
pal = xcosPalAddBlock(pal,"BIGSOM_f");
xcosPalAdd(pal);

exit(0);

// restart scilab

xcos();

// check that "my palette" does not appear anymore

