// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2636 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2636
//
// <-- Short Description -->
//    Scilab is not able to copy a file in an empty Directory.
//

mkdir(TMPDIR+"/bug_2636");
mputl("test",TMPDIR+"/bug_2636.txt");
copyfile(TMPDIR+"/bug_2636.txt",TMPDIR+"/bug_2636/bug_2636.txt");
if mgetl(TMPDIR+"/bug_2636/bug_2636.txt") <> 'test' then pause,end
