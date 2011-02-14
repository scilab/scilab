// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- JVM NOT MANDATORY -->
//
// <-- Non-regression test for bug 8296 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8296
//
// <-- Short Description -->
// size( M,3) crashed scilab 5.3.0-beta-4.

M = ones(3,2);
if execstr("R = size(M, 3);", "errcatch") <> 44 then pause, end
if execstr("R = size(ones(3,2), 3);", "errcatch") <> 44 then pause, end

if size(M, "c") <> 2 then pause, end
if size(M, "r") <> 3 then pause, end
if size(M, "*") <> 6 then pause, end
if size(M ,1) <> 3 then pause,end
if size(M ,2) <> 2 then pause,end
if size(ones(3,2) ,"c") <> 2 then pause,end
if size(ones(3,2) ,"r") <> 3 then pause,end
if size(ones(3,2) ,"*") <> 6 then pause,end
if size(ones(3,2) ,1) <> 3 then pause,end
if size(ones(3,2) ,2) <> 2 then pause,end
if size(ones(3,2) ,1) <> size(ones(3,2) ,"r") then pause,end
if size(ones(3,2) ,2) <> size(ones(3,2) ,"c") then pause,end

