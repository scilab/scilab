// Non-regression test file for bug 1506
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2005
// Date : 28 décembre 2005

mode(-1);
clear

now = getdate();

one_sec = datenum(now(1),now(2),now(3),now(7),now(8),30) -  datenum(now(1),now(2),now(3),now(7),now(8),29);
one_sec_ref = 1/(3600*24);
test1  = (abs( (one_sec - one_sec_ref) / one_sec ) < 1e-5);

one_min = datenum(now(1),now(2),now(3),now(7),18,now(9)) -  datenum(now(1),now(2),now(3),now(7),17,now(9));
one_min_ref = 1/(60*24);
test2  = (abs( (one_min - one_min_ref) / one_min ) < 1e-6);

one_hour = datenum(now(1),now(2),now(3),18,now(8),now(9)) -  datenum(now(1),now(2),now(3),17,now(8),now(9));
one_hour_ref = 1/24;
test3  = (abs( (one_hour - one_hour_ref) / one_hour ) < 1e-8);

if ( test1 & test2 & test3 ) then
	affich_result(%T,1506);
else
	affich_result(%F,1506);
end

clear
