// =============================================================================
// Copyright (C) 2010 - 2012 - INRIA - Allan CORNET
// =============================================================================
// <-- CLI SHELL MODE -->
// =============================================================================
// <-- LONG TIME EXECUTION -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 194 -->
//
// <-- URL -->
// http://forge.scilab.org/index.php/p/csv-readwrite/issues/194/
//
// <-- Short Description -->
// csvRead may fail on large files.
// =============================================================================
filename = fullfile(TMPDIR, "prices.csv");
fd = mopen(filename,"w");
imax=1800000;
for i= 1:imax
    mputl("02/05/10 00:00:02,1.32453,1.32491",fd);
end
mclose(fd);

assert_checkequal(execstr("M = csvRead(filename,[],[],""string"");", "errcatch"), 0);
clear M
// =============================================================================
