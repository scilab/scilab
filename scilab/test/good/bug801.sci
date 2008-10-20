// Non-regression test file for bug 801
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Date : 3 mai 2005

clear;
mode(-1);

x=[4 5];
y=[3 4 5 6];
m=[ 2 3 4 5; 5 4 3 2];

correct=%F

if (covar(x,y,m) <> -0.1785714) then
  correct=%T
end

affich_result(correct,801);

// clear
