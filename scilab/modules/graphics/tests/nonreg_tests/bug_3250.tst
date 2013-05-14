// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- NOT FIXED -->

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3250 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3250
//
// <-- Short Description -->
// Creates more 40 graphic windows crashs scilab.

// first check that here is no memory leaks when opening windows
plot(1:10);
freeMemInit = getmemory();

// create several windows and destroy them
for i=1:100,
  // perform a plot and then destroy the window
  delete(gcf());
  // create a window and instantly destroy it
  delete(gcf());
  plot(1:10);
end

freeMemEnd = getmemory();

// There should not be any memory difference, since we will have the same number of opened window. However we toterate a difference of 10Mb
if ((freeMemInit - freeMemEnd) > 1.0e5) then pause; end

// second test. Open a lot of window. Scilab should provide an error but not crash.

for i=1:300,
 execstr("scf()","errcatch");
end


