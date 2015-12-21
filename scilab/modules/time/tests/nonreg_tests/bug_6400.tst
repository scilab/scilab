// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 6400 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6400
//
// <-- Short Description -->
//
// Msecs not correctly handled in datevec/datenum functions
// Error is raised when datenum input is a time with 59 sec and not null msec.

warning("off"); // WARNING_EMPTY_OPS
if execstr("datenum(2009,5,1,9,58,59.807998);","errcatch")   <> 0 then pause,end
if execstr("datenum([2009 5 1 9 58 59.807998]);","errcatch") <> 0 then pause,end
if execstr("datenum(datevec(733894.41597));","errcatch")     <> 0 then pause,end
