// <-- Non-regression test for bug 1711 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1711
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

// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2005
// Date : 28 décembre 2005

test1 =  ( datenum(2006,1,4) == 732681 );

if test1 then
	affich_result(%T,1711);
else
	affich_result(%F,1711);
end
