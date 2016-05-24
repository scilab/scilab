// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 1505 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1505
//
// <-- Short Description -->
//    datenum does not handle years correctly:
//    example:
//    datenum(1971,1,1) - datenum(1970,1,1)
//    returns 0 on my RedHat Linux x86.
//
//    However
//    datenum(1971,3,1) - datenum(1970,3,1)
//    gives the expected result : 365.2425
//    (The bug only occur for month = 1 and 2)
//
//    Fabian

test1 =  ( (datenum(1971,1,1) - datenum(1970,1,1)) == 365 )
test2 =  ( (datenum(1970,2,1) - datenum(1970,1,1)) == 31 )
test3 =  ( (datenum(1970,1,2) - datenum(1970,1,1)) == 1 )

if ~test1 then pause,end
if ~test2 then pause,end
if ~test3 then pause,end
