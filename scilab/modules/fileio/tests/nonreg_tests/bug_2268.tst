// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2268 -->
// <-- NOT FIXED -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2268
//
// <-- Short Description -->
//    Memory leak in msscanf().  When calling msscanf() many times, as in reading in
//    large CSV files, process size grows beyond memory capacity.

free=getmemory();

for k=1:10000
  foo=msscanf('1.234 44 55 66 77 88 88 ', '%f %f %f %f %f %f %f '); 
end

leak = free-getmemory();
if leak>4  then pause,end
