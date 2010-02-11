// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

//
// <-- Non-regression test for bug 6436 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6436
//
// <-- Short Description -->
// uigetfile(["*.bin";"*.sce";"*.cos"]); did not let me select either of those file extensions.

cd(TMPDIR);
mkdir('bug_6436');
cd('bug_6436');
fd = mopen('bug_6436.bin','wt');
mclose(fd);
fd = mopen('bug_6436.sce','wt');
mclose(fd);
fd = mopen('bug_6436.cos','wt');
mclose(fd);
fd = mopen('bug_6436.xcos','wt');
mclose(fd);
fd = mopen('bug_6436.cosf','wt');
mclose(fd);

uigetfile(["*.bin";"*.sce";"*.cos"],TMPDIR+'/bug_6436')

// change filter selection in uigetdir and try to see if you see all files