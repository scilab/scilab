//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2009 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2588 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2588
//
// <-- Short Description -->
//Calling a function with no input arugment contained in an mlist
//generates an error
// <-- ENGLISH IMPOSED -->

M=[];
function y=foo(a)
  if argn(2)==0 then
    y=%s
  else
    y=a
  end
endfunction
M.fun=foo;
if M.fun()+1<>%s+1 then pause,end
if M.fun(33)+1<>34 then pause,end
clear foo;
function [x,y]=foo(a,b)
  x=a*b
  y=a-b
endfunction
M.fun=foo;
[u,v]=M.fun(2,3);
if u<>2*3|v<>2-3 then pause,end
[u]=M.fun(2,3);
if u<>2*3 then pause,end

N=[];
N.x=M;
if N.x.fun(2,3)<>2*3 then pause,end



%fptr_i_s=generic_i_s;
M.fun=sin;
if M.fun(%pi)<>sin(%pi) then pause,end;

%fptr_i_s=generic_i_s;M=[];
ieee(0);
M.fun=ieee;
if M.fun()<>0 then pause,end


A=rand(2,2);
M.fun=A;
if M.fun()<>A then pause,end


