// <-- Non-regression test for bug 2429 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2429
//
// <-- Short Description -->
//    Implicit vector in for loop are allocated


// Non-regression test file for bug 2429
// Copyright INRIA
// Scilab Project - Serge Steer
// Copyright INRIA 2007
// Date : June 2007

mode(-1) ;
clear ;


S=stacksize();
function test(),for k=1:2d5,end,endfunction

stacksize(1d5)

ierr=execstr('test()','errcatch')
stacksize(S(1))
affich_result(ierr==0,2429) ;
