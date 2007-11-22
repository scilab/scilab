// <-- Non-regression test for bug 1014 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1014
//
// <-- Short Description -->
//    Nan bug (Windows)
//    b=%nan;
//    a=1/b
//
//    produces a "Division by zero" error


// bug1014
// Allan CORNET
clear
a=%nan;
b=1/a;
affich_result( isnan(b) == %T , 1014)
clear


