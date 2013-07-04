// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 11257 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11257
//
// <-- Short Description -->
// Memory leaks in Scilab graphics.

n=1024;
m=1040;
a=rand(n,m);

for i=1:10
  h=scf();
  grayplot(1:n,1:m,a);
  delete(h);
end
