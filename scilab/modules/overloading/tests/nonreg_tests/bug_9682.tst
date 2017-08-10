//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 9682 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9682
//
// <-- Short Description -->
// 3D assignment into a 2D array of int issued an error


i = uint8([1 10;254 9]);
if execstr("i(:,:,2)=uint8(1)","errcatch")<>0 then pause,end;
if ~assert_checkequal(i(:,:,2),uint8([1 1;1 1])) then pause,end;
