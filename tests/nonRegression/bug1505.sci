// Non-regression test file for bug 1505
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2005
// Date : 28 décembre 2005

mode(-1);
clear

test1 =  ( (datenum(1971,1,1) - datenum(1970,1,1)) == 365 )
test2 =  ( (datenum(1970,2,1) - datenum(1970,1,1)) == 31 ) 
test3 =  ( (datenum(1970,1,2) - datenum(1970,1,1)) == 1 )

if ( test1 & test2 & test3 ) then
	affich_result(%T,1505);
else
	affich_result(%F,1505);
end

clear
