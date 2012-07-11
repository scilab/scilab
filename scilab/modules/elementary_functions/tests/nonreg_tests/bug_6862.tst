// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 6862 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6862
//
// <-- Short Description -->
// vectorfind(M,V,dir)
// * The number of mandatory entries is not checked.
// * typeof(M)==typeof(V) is not checked.

// Check that at least 2 inputs are needed
ierr = execstr("vectorfind([2 2])", "errcatch");
errmsg = msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"), "vectorfind", 2, 3);
if ierr==0 | (lasterror() <> errmsg) then pause; end

// Check that "r" option is added by default
if vectorfind([2 2], [2 2]) <> vectorfind([2 2], [2 2], "r") then pause; end

// Check that an error message is returned if first and second input arguments have not the same type
ierr = execstr("vectorfind([1 2], string([1 2]))", "errcatch");
errmsg = msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same type expected.\n"), "vectorfind", 1, 2);
if ierr==0 | lasterror() <> errmsg then pause; end
