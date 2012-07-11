// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 6952 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6952
//
// <-- Short Description -->
// stacksize('max') did not return always maximum value.

r = stacksize();
stacksize('max');
mx = stacksize();

for i = 1:100
  stacksize('max');
  m = stacksize();
  if m(1) <> mx(1) then pause, end
end