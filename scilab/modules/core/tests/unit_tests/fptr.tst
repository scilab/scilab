//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->

//tests on functions pointer usage

funcprot(0);

f=exp
if type(f)<>130 then pause,end
if f(1)<>exp(1) then pause,end
l=list(exp,f,3);
f=l(1);
if type(f)<>130 then pause,end
if f(1)<>exp(1) then pause,end

f=l(2);
if type(f)<>130 then pause,end
if f(1)<>exp(1) then pause,end

l=tlist(['foo','fun'],exp);
f=l.fun;
if type(f)<>130 then pause,end
if f(1)<>exp(1) then pause,end

if l.fun(1)<>exp(1) then pause,end

function y=myf(x),y=exp(x),endfunction
l=tlist(['foo','fun'],myf);
if l.fun(1)<> exp(1) then pause,end


function y=foo(fun,x),y=fun(x),endfunction
if foo(exp,1)<>exp(1) then pause,end


function y=foo(fun,x,y),y=fun(x,y),endfunction
if foo(atan,1,0)<>atan(1,0) then pause,end

function y=foo(fun),y=[],for x=1:3,y=[y,fun(x)],end,endfunction
if foo(exp)<>exp(1:3) then pause,end

l=tlist(['foo','funs'],list(exp,sin,cos));
function y=foo(l),y=[],for k=1:3,y=[y,l.funs(k)(1)],end,endfunction
if or(foo(l)<>[exp(1),sin(1),cos(1)]) then pause,end
