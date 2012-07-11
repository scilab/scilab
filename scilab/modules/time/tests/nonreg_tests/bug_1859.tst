// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 1859 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1859
//
// <-- Short Description -->
//    I sent the following commands
//
//    Year = [2002, 2002];
//    Month = [2, 3];
//    Day = [28, 1];
//    datenum(Year, Month, Day)
//
//    The SCILAB output is:
//
//      731275. 731278.
//
//    But this seems me incorrect, as 2002-03-01 is just one day following 2002-02-28.
//
//    Now, if you give the commands
//
//      datenum(2002,2,28)
//      datenum(2002,3,1)
//
//    you respectively obtain the two outputs
//
// ...

Year  = [2002, 2002];
Month = [   2,    3];
Day   = [  28,    1];

test  = datenum(Year, Month, Day);
ref   = [731275 731276];

if or(test <> ref) then pause,end
