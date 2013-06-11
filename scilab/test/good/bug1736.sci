// Non-regression test file for bug 1736
// Copyright INRIA
// Scilab Project - Serge Steer
// Copyright INRIA 2006
// Date : 11 janvier 2006

mode(-1);
clear
a = uint8(rand(10,10)*100);
ok=(a<=50)==(50>=a)
affich_result(ok,1736);

clear
