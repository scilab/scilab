// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

// <-- Non-regression test for bug 3896 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3896
//
// <-- Short Description -->
// According to the "mputl" online help [1], If the file descriptor is omitted, lines are written in the last file opened by mopen.
// If only one file is opened in Scilab, this syntax fails.

warning('off');

fd1 = mopen(TMPDIR+'/text_1.txt','w');
if mputl('Hello world') <> %t then pause,end;

fd2 = mopen(TMPDIR+'/text_2.txt','w');
if mputl('Hello world') <> %t then pause,end;

mclose (fd1);
mclose (fd2);

fd1 = mopen(TMPDIR+'/text_1.txt','w');
fd2 = mopen(TMPDIR+'/text_2.txt','w');

if mputl('Text') <> %t then pause,end;
if mputl('Text') <> %t then pause,end;

fd3 = mopen(TMPDIR+'/text_3.txt','w');

if mputl('Text') <> %t then pause,end;

mclose (fd1);
mclose (fd2);
mclose (fd3);

