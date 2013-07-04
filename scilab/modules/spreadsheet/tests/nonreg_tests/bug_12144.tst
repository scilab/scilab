// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Simon MARCHETTO
//
// This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 12144 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12144
//
// <-- Short Description -->
// csvRead triggers some errors about memory


CSV = ["1,0,0,0,0"; ..
"0,1,0,0,0"; ..
"0,0,1,0,0"; ..
"4,4,1,2,0"; ..
"4,63,1,2,0"; ..
"4,63,1,2,233"; ..
"42,3,23,2,233"; ..
];
filename = fullfile(TMPDIR, 'foo.csv');
mputl(CSV, filename);
instr = "r = csvRead(filename, "" "", [], ""string"", [], [], [5 3 7 6]);";
refMsg = msprintf(gettext("%s: Range row or/and column left indice(s) out of bounds.\n"), "csvRead");
assert_checkerror(instr, refMsg);
mdelete(filename);
