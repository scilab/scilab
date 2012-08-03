// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 3830 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3830
//
// <-- Short Description -->
// mputl fails to write some accents

ref = [194,176];
fd = mopen(TMPDIR + "/toto","wb");
mput(ref, 'uc', fd);
mclose(fd);
fd = mopen(TMPDIR + "/toto", "rb");
res = mget(2,'uc',fd);
if or(ref<>res) then pause,end
