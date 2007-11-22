// <-- Non-regression test for bug 1625 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1625
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


// Non-regression test file for bug 1625
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
