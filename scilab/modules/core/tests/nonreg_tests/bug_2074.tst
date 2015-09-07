//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2074 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2074
//
// <-- Short Description -->
// incompatible LHS in assignment submatrix=function result

function [a,b]=illegalLHS()
    a=[1;1]; b=[2;2]
endfunction

if or(illegalLHS()<>[1;1]) then pause,end
[a,b]=illegalLHS();
if or(a<>[1;1]) then pause,end
A=zeros(2,2); A(:,1)=illegalLHS();
if or(A<>[1 0;1 0]) then pause,end

A=zeros(2,2); [a,A(:,1)]=illegalLHS();
if or(A<>[2 0;2 0])|or(a<>[1;1]) then pause,end


A=zeros(2,2); [A(:,1)]=illegalLHS();
if or(A<>[1 0;1 0]) then pause,end

A=zeros(2,2); [A(:,1),b]=illegalLHS();
if or(A<>[1 0;1 0])|or(b<>[2;2])  then pause,end
