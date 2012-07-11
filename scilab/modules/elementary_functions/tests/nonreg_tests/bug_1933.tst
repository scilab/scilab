// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 1933 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1933
//
// <-- Short Description -->
//    log and log2 doesn't work on vectors with %nan under windows. They work 
//    nicely
//    under linux.

A             = [0 0 2 2 0 0];
A(find(A==0)) = %nan;
B             = log(A);

if ~(isnan(B(1))) then pause,end
if ~(isnan(B(2))) then pause,end
if ~(isnan(B(5))) then pause,end
if ~(isnan(B(6))) then pause,end

