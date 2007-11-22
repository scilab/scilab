// <-- Non-regression test for bug 1227 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1227
//
// <-- Short Description -->
//    -->a=1;savematfile('file','a')
//    WARNING:Option -v6 added
//     !--error   246 
//    function not defined for given argument type(s)
//    Check arguments or define function %s_strindex             
//    for overloading
//    at line     287 of function savematfile called by :  
//    savematfile('totoo','a')


//***********************************************************************************************************************
// File : bug 1227
// Author : Pierre MARECHAL
// Last modified : 2 Mar 2005 
//***********************************************************************************************************************
 
a = 123456789;
savematfile(TMPDIR+'/file_1232.mat','a');
clear a
loadmatfile(TMPDIR+'/file_1232.mat','a');

if( a == 123456789 ) then
	affich_result(%T, 1227)
else
	affich_result(%F, 1227)
end