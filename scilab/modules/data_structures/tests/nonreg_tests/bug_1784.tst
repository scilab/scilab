//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1784 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1784
//
// <-- Short Description -->
//    Problem while inserting a matrix in an N-D matrix

A = zeros(3,3,2);

try
	A(1,1:3,1:3)=[1 2 3;4 5 6;7 8 9];
catch
	if %T then pause,end
	return;
end

if or(A<>matrix([1;0;0;4;0;0;7;0;0;2;0;0;5;0;0;8;0;0;3;0;0; ...
			6;0;0;9;0;0], [3,3,3])) then pause,end
