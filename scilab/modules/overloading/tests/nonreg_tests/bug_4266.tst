//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4266 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4266
//
// <-- Short Description -->
// ones, rand, zeros functions do not remove highest dimensions equal to 1.
a=ones(2,2,1);
if or(size(a)<>[2 2]) then pause,end
a=ones(2,2,1,1);
if or(size(a)<>[2 2]) then pause,end
if or(a<>ones(2,2)) then pause,end
a=ones(2,2,2);
if or(size(a)<>[2 2 2]) then pause,end

a=zeros(2,2,1);
if or(size(a)<>[2 2]) then pause,end
a=zeros(2,2,1,1);
if or(size(a)<>[2 2]) then pause,end
if or(a<>zeros(2,2)) then pause,end
a=zeros(2,2,2);
if or(size(a)<>[2 2 2]) then pause,end

a=rand(2,2,1);
if or(size(a)<>[2 2]) then pause,end
a=rand(2,2,1,1);
if or(size(a)<>[2 2]) then pause,end
if or(a<>a(:,:)) then pause,end
a=rand(2,2,2);
if or(size(a)<>[2 2 2]) then pause,end


a=rand(2,2,1,'normal');
if or(size(a)<>[2 2]) then pause,end
a=rand(2,2,1,1,'normal');
if or(size(a)<>[2 2]) then pause,end
if or(a<>a(:,:)) then pause,end
a=rand(2,2,2,'normal');
if or(size(a)<>[2 2 2]) then pause,end


