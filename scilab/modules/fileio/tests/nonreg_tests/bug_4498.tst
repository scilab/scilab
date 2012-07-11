// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2009 - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 4498 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4498
//
// <-- Short Description -->
//  merror() returns weird strings if called before opening files.

<-- INTERACTIVE TEST -->

[a,b] = merror()

// checks value of a & b

// it should be something as :
// a = 22
// b = "Invalid argument" a string localized by O.S