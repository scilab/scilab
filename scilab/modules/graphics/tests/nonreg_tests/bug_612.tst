// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 612 -->
// <-- NOT FIXED -->
// <-- TEST WITH GRAPHIC -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=612
//
// <-- Short Description -->
//    under unix the dimension of graphic window is limited to 
//    2^16, if one set dimensions higher than this the effective
//    dimension,  the value really assigned is modulo 2^16
//
//    The pb does not exist under windows

xset("wresize",0);
xset('wdim',2^16+3,5000);

if getos() <> 'Windows' then
	resultLinux   = xget("wdim");
	resultWindows = [ 65535.    5000.];
	if or(resultLinux <> resultWindows) then pause,end
end
