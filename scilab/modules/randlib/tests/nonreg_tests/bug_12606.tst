// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 12606 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12606
//
// <-- Short Description -->
// overloads names for grand() are not standard: %#_Rand() instead of %#_grand()

// Save warning mode and turn off warnings
status = warning("query");
warning("off");

function r=%b_grand(arg)
    r="boolean overload for grand is %b_grand() as expected";
endfunction
assert_checkequal(grand(%t),"boolean overload for grand is %b_grand() as expected");

// Revert to the previous warning mode
warning(status);
