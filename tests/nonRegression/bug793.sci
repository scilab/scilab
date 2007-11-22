// <-- Non-regression test for bug 793 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=793
//
// <-- Short Description -->
//    Getdate crashes scilab with negative arguments


// bug793

mode (-1);
clear

affich_result(getdate(-1) == [0 0 0 0 0 0 0 0 0 0], 793)

//-->getdate(-1)
//dt=getdate(x) x must be >0.
// ans  =
//
//!   0.    0.    0.    0.    0.    0.    0.    0.    0. !  
clear

