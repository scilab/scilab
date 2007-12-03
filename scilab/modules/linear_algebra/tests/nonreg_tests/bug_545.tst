// <-- Non-regression test for bug 545 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=545
//
// <-- Short Description -->
//    Bug Report Id: 031202003109134720
//    This is obviously a conflict between a local variable ndims and the new function ndims(). 
//    I expect the same problem also in: 
//
//    macros/percent/%b_i_hm.sci
//    macros/percent/%hm_i_hm.sc
//    macros/percent/%p_i_hm.sci
//
//    Wasn't this an already solved issue??
//    On Scilab CVS with " %i_i_hm   " function
//    The Error Messages are:
//     Warning :redefining function: ndims                   
//             inside function: %i_i_hm                 
//
//    Commands: 
//    a=int8(hypermat({3 2 2}));b=hypermat({1 2 2});
//    a(1,:,:)=b
//    Enrico SEGRE on Linux version RH9 distribution  with   as window manager
//    Israel  
//    November 9, 2003 at 13:47:20


// Non-regression test file for bug 545
// Copyright INRIA
// Scilab Project
// Date : 9 Dec 2005

my_dia_file = TMPDIR+"/bug545.dia";

diary(my_dia_file);
a=int8(hypermat({3 2 2}));
b=hypermat({1 2 2});
a(1,:,:)=b;
diary(0);

my_dia_content = mgetl(my_dia_file);
if grep(my_dia_content,"redefining") <> [] then pause,end
