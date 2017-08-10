// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 9250 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9250
//
// <-- Short Description -->
// mkdir should fail if we give more than one directory
//

try
  mkdir(["a", "b"])
catch
  [str,n]=lasterror();
  if n <> 999 then pause, end
  if str <> msprintf(_("%s: Wrong size for input argument #%d: string expected.\n"), "mkdir", 1) then pause, end
end