// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 1625 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1625
//
// <-- Short Description -->
//    This problem concerns th datenum function:
//    I'm getting the following results
//
//    -->datenum(1996,12,31)
//     ans  =
//
//        729389.8
//
//    -->datenum(1997,1,1)
//     ans  =
//
//        1.5

if datenum(1997,1,1) <> 729391 then pause,end
if datenum(1996,12,31) <> floor(datenum(1996,12,31)) then pause,end
if datenum(1997,1,1,23,59,59) - datenum(1997,1,1) >= 1 then pause,end
