// Non-regression test file for bug 2067
// Copyright INRIA
// Scilab Project - Jean-Baptiste Silvy
// Copyright INRIA 2006
// Date : July 11 2006

mode(-1) ;
clear ;

// the bug was hanging the computer
plot( 10^16, 5) ;

affich_result(%T,2067) ;
clear ;
