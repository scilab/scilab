// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 2458 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2458
//
// <-- Short Description -->
//Under linux Suse 10.2, with build4, official build of 4.1.1
// and 4.1 there is a memory leak relative to show_pixmap()
// memory result (ram dependency)
nbComputation = 2000;

plot2d();
f=gcf();f.pixmap='on';
show_pixmap(); // show it once for memory allocation

freeMemoryInit = getmemory();
for k=1:nbComputation, show_pixmap(); end 

// compute used memory during compuations
usedMemory = freeMemoryInit - getmemory();

// 5 mega
if (usedMemory > 5000) then pause; end

