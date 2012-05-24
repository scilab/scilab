// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 5263 -->
//
// <-- Short Description -->
// [id, filenames]=diary() returns incorrect paths for automatically numbered filenames
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5263
//

cd TMPDIR;
[a0, f0] = diary();
i1 = diary("journal.txt","new","filter=command" );
[a1, f1] = diary();
i2 = diary("journal.txt","new","filter=command" );
[a2, f2] = diary();

diary(i1,'close');
diary(i2,'close');

if grep(f2, strsubst(TMPDIR+'/journal_0.txt','\','/')) == 0 then pause,end
