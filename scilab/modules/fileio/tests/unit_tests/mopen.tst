// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2021 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

fd = mopen(TMPDIR + filesep() + ['mclose_tst_1','mclose_tst_2'],'wt');
mputl("test string 1",fd(1));
mputl("test string 2",fd(2));
mclose(fd);

fd = mopen(TMPDIR + filesep() + ['mclose_tst_1','mclose_tst_2']);
str = [mgetl(fd(1)),mgetl(fd(2))];
mclose(fd);

assert_checkequal(str,["test string 1","test string 2"]);