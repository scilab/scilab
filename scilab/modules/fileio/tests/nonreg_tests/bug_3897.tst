// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 3897 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3897
//
// <-- Short Description -->
// macros perturbs the behavior of mfprintf().

fd = mopen(TMPDIR+'/text_1.txt','wt');
mfprintf(-1,'Hello World 1\n');
r = sind(90);
mfprintf(-1,'Hello World 2 \n'); 
mclose(fd);

fd = mopen(TMPDIR+'/text_1.txt','rt');
ierr = execstr("mfprintf(fd,''Hello World 3\n'');","errcatch");
if ierr <> 999 then pause,end


