// <-- Non-regression test for bug 654 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=654
//
// <-- Short Description -->
//    listfiles(getenv('SCI')+"/tcl") gives wrong result. 
//
//    - or the syntax is accepted and the result should be correct
//    - or an error message should be displayed
//
//    of course listfiles(getenv('SCI')+"/tcl/") is OK


// bug654
// 

mode (-1)
clear
affich_result( and(listfiles(getenv('SCI')+"/tcl") == listfiles(getenv('SCI')+"/tcl/")) , 654)
clear


