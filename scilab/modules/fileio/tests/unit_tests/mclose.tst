// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

fd = mopen(TMPDIR + filesep() + 'mclose_tst','wt');
mclose(TMPDIR + filesep() + 'mclose_tst');

ierr = execstr('mclose([''file1'',''file2''])','errcatch');
if ierr <> 999 then pause,end

ierr = execstr('mclose([1,2])','errcatch');
if ierr <> 999 then pause,end

mclose('all');

// Check that mclose('all') closes all files (except stdin and stdout)
f1 = mopen(TMPDIR + filesep() + "mclose1.txt","w");
f2 = mopen(TMPDIR + filesep() + "mclose2.txt","w");
mclose("all");

if size(file())<>2 then pause; end
