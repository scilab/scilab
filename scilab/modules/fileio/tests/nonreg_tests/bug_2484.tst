// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2484 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2484
//
// <-- Short Description -->
//

a    = list();
a(2) = 10;
save(TMPDIR+"/test.sav","a");
if execstr("listvarinfile(TMPDIR+""/test.sav"");", "errcatch")<>0 then pause; end
