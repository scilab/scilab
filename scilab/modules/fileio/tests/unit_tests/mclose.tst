// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

fd(1) = mopen(TMPDIR + filesep() + 'mclose_tst_1','wt');
fd(2) = mopen(TMPDIR + filesep() + 'mclose_tst_2','wt');

mclose(fd);

mopen(TMPDIR + filesep() + 'mclose_tst_1','wt');
mopen(TMPDIR + filesep() + 'mclose_tst_2','wt');

mclose(TMPDIR + filesep() + ['mclose_tst_1','mclose_tst_2']);

mopen(TMPDIR + filesep() + 'mclose_tst_1','wt');
mopen(TMPDIR + filesep() + 'mclose_tst_2','wt');

mclose("all")

ierr = execstr('mclose(0)','errcatch');
if ierr <> 999 then pause,end
ierr = execstr('mclose(5)','errcatch');
if ierr <> 999 then pause,end
ierr = execstr('mclose(6)','errcatch');
if ierr <> 999 then pause,end
