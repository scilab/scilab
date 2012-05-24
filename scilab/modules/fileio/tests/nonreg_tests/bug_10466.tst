// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- LONG TIME EXECUTION -->
//
// <-- Non-regression test for bug 10466 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10466
//
// <-- Short Description -->
// Bug #10466 fixed - mgetl detected an error but it did not return a error in scilab.
//

filename = fullfile(TMPDIR, "bug_10466.csv");
mprintf("File=%s\n", filename);
fd = mopen(filename,"wt");
imax=1800000;
for i= 1:imax
  if ( modulo(i,10000)==0 ) then
    mprintf("i=%d, p=%.1f%%\n",i,i/imax*100)
  end
  mputl("02/05/10 00:00:02,1.32453,1.32491",fd);
end
mclose(fd);

assert_checkequal(execstr("M = mgetl(filename);", "errcatch"), 17);
