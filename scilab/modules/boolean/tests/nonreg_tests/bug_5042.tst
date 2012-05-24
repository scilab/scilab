// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 5042 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5042
//
// <-- Short Description -->
// (1|[1,0,1;1,1,0]) returns incorrect values.

ref_or = [%t, %t, %t;%t, %t, %t];
res = (1|[1,0,1;1,1,0]);
if ~and(ref_or == res) then pause,end

ref_or = [%t, %t, %t;%t, %t, %t];
res = (1|[%t,%f,%t;%t,%t,%f]);
if ~and(ref_or == res) then pause,end

ref_and = [%t %f %t; %t %t %f];
res = (1&[1,0,1;1,1,0]);
if ~and(ref_and == res) then pause,end

ref_and = [%t %f %t; %t %t %f];
res = (1&[%t,%f,%t;%t,%t,%f]);
if ~and(ref_and == res) then pause,end

ref_or = [%t, %t, %t;%t, %t, %t];
res = ([1,0,1;1,1,0]|1);
if ~and(ref_or == res) then pause,end

ref_and = [%t %f %t; %t %t %f];
res = ([1,0,1;1,1,0]&1);
if ~and(ref_and == res) then pause,end

ref_or = [%T %F %T;%T %T %F];
res = ([1,0,1;1,1,0]|[1,0,1;1,1,0]);
if ~and(ref_or == res) then pause,end

ref_and = [%T %F %T; %T %T %F];  
res = ([1,0,1;1,1,0]&[1,0,1;1,1,0]);
if ~and(ref_and == res) then pause,end
