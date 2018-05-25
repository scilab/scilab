// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// ===========================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 8810 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8810
//
// <-- Short Description -->
// Insertions with an hypermatrix of indices failed

a = 5;
assert_checkequal(a(ones(2,2,2)), 5*ones(2,2,2));
