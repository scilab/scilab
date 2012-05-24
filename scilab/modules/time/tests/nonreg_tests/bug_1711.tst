// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 1711 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1711
//
// <-- Short Description -->
//    datenum() produces incorrect results. For example:
//
//    -->datenum(2006,1,4)
//     ans  =
//
//        4.5
//
//    -->datenum()
//     ans  =
//
//        4.8753166
//
//    -->datevec(datenum())
//     ans  =
//
//      - 1.    13.    4.    21.    0.    27.783333

if datenum(2006,1,4) <> 732681 then pause,end
