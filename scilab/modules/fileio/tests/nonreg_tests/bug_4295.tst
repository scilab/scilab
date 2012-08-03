// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 4295 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4295
//
// <-- Short Description -->
// The function mdelete no more works with "*.extension" as input, it works in previous versions (Scilab 4.1.2 for exemple).

fd = mopen(TMPDIR + filesep() + "file1.bug","wt");
mclose(fd);
fd = mopen(TMPDIR + filesep() + "file2.bug","wt");
mclose(fd);
fd = mopen(TMPDIR + filesep() + "file3.bug","wt");
mclose(fd);
mdelete(TMPDIR + filesep() + "*.bug");
if listfiles(TMPDIR + filesep() + "*.bug") <> [] then pause,end
