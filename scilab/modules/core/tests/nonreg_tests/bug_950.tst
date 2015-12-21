// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- NOT FIXED -->
// <-- Non-regression test for bug 950 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=950
//
// <-- Short Description -->
//    extra character in "select" line crashes Scilab instead of 
//    issuing a "syntax error" message.

try
	function [] = Select (Type)
	select Type, n
		case 1 then
			disp ("1")
		case 2 then
			disp ("2")
	end;
	endfunction;
	
	if %T then pause,end
	
catch
	
end
