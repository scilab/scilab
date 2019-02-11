// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- WINDOWS ONLY -->
//
// <-- Non-regression test for bug 15206 -->
//
// <-- Bugzilla URL -->
// https://bugzilla.scilab.org/show_bug.cgi?id=15206
//
// <-- Short Description -->
// wrong writing with mput

filename = fullfile(TMPDIR,'test.bin');
fid = mopen(filename,'w+');
mput (int16([1:10]), 's', fid ) ;
offset = mtell(fid) ;
mclose(fid);
deletefile(filename);
assert_checkequal(offset,20);

