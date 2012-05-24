// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 1506 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1506
//
// <-- Short Description -->
//    datevec(datenum()) gives wrong result.
//
//    datevec(datenum()) is supposed to give the actual time, right?
//    On my box (Redhat Linux x86) it now gives
//    !   2005.    7.    27.    15.    10.    50.249993 !
//
//    where the date on a unix shell gives:
//    Wed Jul 27 12:02:16 CEST 2005
//
//    So the date is correct but not the time
//
//    Fabian

now_date = getdate();

one_sec = datenum(now_date(1),now_date(2),now_date(6),now_date(7),now_date(8),30) -  datenum(now_date(1),now_date(2),now_date(6),now_date(7),now_date(8),29);
one_sec_ref = 1/(3600*24);
test1  = (abs( (one_sec - one_sec_ref) / one_sec ) < 1e-5);

one_min = datenum(now_date(1),now_date(2),now_date(6),now_date(7),18,now_date(9)) -  datenum(now_date(1),now_date(2),now_date(6),now_date(7),17,now_date(9));
one_min_ref = 1/(60*24);
test2  = (abs( (one_min - one_min_ref) / one_min ) < 1e-6);

one_hour = datenum(now_date(1),now_date(2),now_date(6),18,now_date(8),now_date(9)) -  datenum(now_date(1),now_date(2),now_date(6),17,now_date(8),now_date(9));
one_hour_ref = 1/24;
test3  = (abs( (one_hour - one_hour_ref) / one_hour ) < 1e-8);

if ~test1 then pause,end
if ~test2 then pause,end
if ~test3 then pause,end
