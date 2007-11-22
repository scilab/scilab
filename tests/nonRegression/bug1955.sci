// <-- Non-regression test for bug 1955 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1955
//
// <-- Short Description -->
//    Reshaping global hypermatrices is problematic, as demonstrated by the 
//    attached file 'bug.sci', which contains further comments.


// Non-regression test file for bug 1955
// Copyright INRIA
// Scilab Project - Serge Steer
// Copyright INRIA 2006
// Date : 3 mai 2006

mode(-1);
clear

global ghm

ghm=matrix(1:8,2,4);
ghm=matrix(ghm,2,2,2);

r= and(ghm==matrix(1:8,2,2,2))
affich_result(r,1955);
clear
