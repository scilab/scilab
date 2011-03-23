// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 8764 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8764
//
// <-- Short Description -->
// File permissions were not checked 

tmpfile = TMPDIR + "/testwrite.sce";
mputl("disp(""hello"");", tmpfile);

if getos() <> 'Windows' then
  unix("chmod u-w " + tmpfile);
else
  dos("attrib +R " + tmpfile);  
end
scinotes(tmpfile);

// The file must be open in read-only mode

tmpfile = TMPDIR + "/testread.sce";
mputl("disp(""hello"");", tmpfile);

if getos() <> 'Windows' then
  unix("chmod u-r " + tmpfile);
  scinotes(tmpfile);
end

// on Windows, try to open a file without rights on it.

// The file must not be opened !
