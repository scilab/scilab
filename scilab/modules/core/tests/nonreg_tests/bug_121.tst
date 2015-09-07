//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 121 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=121
//
// <-- Short Description -->
//    If I load my file and call function main1() I get a message: undefined variable : y at line 3 of function expcdf1 ... 

function [y]=expcdf1(x,par); // lambda,x > 0
lambda=par(1); y=1-exp(-lambda*x);
endfunction

function [y]=expcdf2(x,par);
lambda=par(1); y=1-exp(-lambda*x);
endfunction

function main1();
x=[1,2,3];
par=[1];
y=expcdf1(x,par);
endfunction

function main2();
x=[1,2,3];
par=[1];
y=expcdf2(x,par);
endfunction

if execstr("main1();", "errcatch")<>0 then pause,end
