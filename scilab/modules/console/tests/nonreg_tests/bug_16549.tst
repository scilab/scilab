// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Stéphane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16549 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16549
//
// <-- Short Description -->
// Simple script crashes Scilab in GUI mode
//

// line must end with CR+LF

mputl("l=;"+ascii([13 10]),TMPDIR+"/test.sce");
err = exec(TMPDIR+"/test.sce", "errcatch", -1);
assert_checktrue(err <> 0);