// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// ===========================================================================
//
// <-- Non-regression test for bug 9246 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9246
//
// <-- Short Description -->
//It is not possible to insert an integer in an integer array with a 
//different integer type

a=int8(ones(2,2));
if execstr("a(1,1)=uint8(2)","errcatch","n") <> 0 then pause,end

