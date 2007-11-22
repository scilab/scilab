// <-- Non-regression test for bug 1742 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1742
//
// <-- Short Description -->
//    function [m]=msd(x,orien)
//    //This function computes  the mean squared  deviation of the values of a
//    //vector or matrix x.
//
//    elseif orien=='r'|orien==1 then
//        m=sqrt(sum((x-ones(x(:,1))*mean(x,'r')).^2,'r')/ncol);
//
//    This formula is wrong. Must be canged in:
//    m=sqrt(sum((x-ones(x(:,1))*mean(x,'r')).^2,'r')/nrow);


// Non-regression test file for bug 1742
// Copyright INRIA
// Scilab Project - Serge Steer
// Copyright INRIA 2006
// Date : 4 mai 2006

mode(-1);
x=matrix([5 2:8],2,4);
r=(msd(x)-3.5)<%eps
r=r&norm(msd(x,1)-[3 1 1 1]/2)<%eps
r=r&norm(msd(x,'r')-[3 1 1 1]/2)<%eps
r=r&norm(msd(x,2)-sqrt([2;5]))<%eps
r=r&norm(msd(x,'c')-sqrt([2;5]))<%eps

affich_result(r,1742);
clear
