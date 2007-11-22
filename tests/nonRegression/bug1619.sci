// <-- Non-regression test for bug 1619 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1619
//
// <-- Short Description -->
//    macr2tree doesn't like functions compiled for profiling. It hates them 
//    so much,
//    that causes spectacular crashes of scilab.


// Non-regression test file for bug 1619
// Copyright INRIA
// Scilab Project - Serge Steer
// Copyright INRIA 2006
// Date : 2 octobre 2006

mode(-1);
clear;



deff('x=foo(n)',['if n==0 then'
                 '  x=[]'
                 'else'
                 '  x=0'
                 '  for k=1:n'
                 '    s=svd(rand(n+10,n+10))'
                 '    x=x+s(1)'
                 '  end'
                 'end'],'p')
T1=execstr('T=macr2tree(foo)','errcatch')==0;
clear foo;
function y=foo(),y=sin(33)+1;endfunction
foo(); //execution creates a deleted op in the pseudo code of foo
T2=execstr('macr2tree(foo)','errcatch')==0
affich_result(T1&T2,1619);

clear

