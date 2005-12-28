// Non-regression test file for bug 1506
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2005
// Date : 28 décembre 2005

mode(-1);
clear

test1 =  ( datenum(1997,1,1) == 729391 );
test2 =  ( datenum(1996,12,31) == floor(datenum(1996,12,31)) );
test3 = ( datenum(1997,1,1,23,59,59) - datenum(1997,1,1) < 1 )

if ( test1 & test2 & test3 ) then
	affich_result(%T,1625);
else
	affich_result(%F,1625);
end

clear
