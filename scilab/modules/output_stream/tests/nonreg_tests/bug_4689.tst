// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 4689 -->
//
// <-- Short Description -->
// diary() do not report interactive commands into the output file
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4689
//

// copy and paste these lines:
fileout = pathconvert(TMPDIR+"/bug_diary.txt",%F);
id = diary(fileout);
disp("Hello world");
diary(id, 'close');
mgetl(fileout)


