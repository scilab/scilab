// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2009 - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 4604 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4604
//
// <-- Short Description -->
//  rmdir(<dir>,"s") doesn't work if it contains a hidden file (starting with a dot).


this_bug_dir  = pathconvert(TMPDIR+"/bug_rmdir",%F);
this_bug_file = pathconvert(this_bug_dir+"/.bugtxt",%F);

r = mkdir(this_bug_dir);
if r <> 1 then pause,end
mputl("some text",this_bug_file);
r = rmdir(this_bug_dir,"s");
if r <> 1 then pause,end
