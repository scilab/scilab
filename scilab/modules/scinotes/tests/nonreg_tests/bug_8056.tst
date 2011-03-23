// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 8056 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8056
//
// <-- Short Description -->
// Bad colorization of the comments after a command without parenthesis.

tmpfile = TMPDIR+'/bug_8056.sci';
fd = mopen(tmpfile,'wt');
mputl('cd ""directory"" /abc //comment',fd);
mclose(fd);
editor(tmpfile)

// the string //comment must be green and /abc must be "pink"
