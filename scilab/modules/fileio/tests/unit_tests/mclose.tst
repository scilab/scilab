// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

fd = mopen(TMPDIR + filesep() + 'mclose_tst','wt');
mclose(TMPDIR + filesep() + 'mclose_tst');

ierr = execstr('mclose([''file1'',''file2''])','errcatch');
if ierr <> 999 then pause,end

ierr = execstr('mclose([1,2])','errcatch');
if ierr <> 999 then pause,end

ierr = execstr('mclose(0)','errcatch');
if ierr <> 999 then pause,end
ierr = execstr('mclose(5)','errcatch');
if ierr <> 999 then pause,end
ierr = execstr('mclose(6)','errcatch');
if ierr <> 999 then pause,end
