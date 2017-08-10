//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA -Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1742 -->
//
// <-- ENGLISH IMPOSED -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1742
//
// <-- Short Description -->
//    function [m]=stdev(x,orien)
//    //This function computes  the mean squared  deviation of the values of a
//    //vector or matrix x.
//
//    elseif orien=='r'|orien==1 then
//        m=sqrt(sum((x-ones(x(:,1))*mean(x,'r')).^2,'r')/ncol);
//
//    This formula is wrong. Must be canged in:
//    m=sqrt(sum((x-ones(x(:,1))*mean(x,'r')).^2,'r')/nrow);

x=matrix([5 2:8],2,4);

if (stdev(x)-3.5)                  >= %eps then pause,end
if norm(stdev(x,1,mean(x, 1))-[3 1 1 1]/2)    >= %eps then pause,end
if norm(stdev(x,"r",mean(x, 1))-[3 1 1 1]/2)  >= %eps then pause,end
if norm(stdev(x,2,mean(x, 2))-sqrt([2;5]))    >= %eps then pause,end
if norm(stdev(x,"c",mean(x, 2))-sqrt([2;5]))  >= %eps then pause,end
