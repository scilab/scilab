// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre Lando
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 5702 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5702
//
// <-- Short Description -->
// Calling legend disables any previous call to drawlater

// Run the following :

T = 0:.1:3;
NbCol = 3;
NbLig = 5;
scf(1);clf;
drawlater()
for s = 1:NbCol
  for n=1:NbLig
    subplot(NbLig, NbCol, (n-1)*NbCol + s);
    plot(T, sin(n.*T))
    legend("x")
  end    
end 
drawnow()

