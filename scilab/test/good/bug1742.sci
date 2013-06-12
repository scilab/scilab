// Non-regression test file for bug 1742
// Copyright INRIA
// Scilab Project - Serge Steer
// Copyright INRIA 2006
// Date : 4 mai 2006

mode(-1);
x=matrix([5 2:8],2,4);
r=(msd(x)-3.5)<%eps
r=r&norm(msd(x,1)-[3 1 1 1]/2)<%eps
r=r&norm(msd(x,"r")-[3 1 1 1]/2)<%eps
r=r&norm(msd(x,2)-sqrt([2;5]))<%eps
r=r&norm(msd(x,"c")-sqrt([2;5]))<%eps

affich_result(r,1742);
clear
