// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// Without arguments
computed=ones();
expected=1.;
if norm(computed-expected)>%eps then pause, end
// With one integer value
computed=ones(10);
expected=[1. 1. 1. 1. 1. 1. 1. 1. 1. 1. 1.];
if norm(computed-expected)>%eps then pause, end
// With two integer values
computed=ones(2,3);
expected=[1. 1. 1.
1. 1. 1.];
if norm(computed-expected)>%eps then pause, end
// With three integer values
computed=ones(2,3,4);
expected=[1. 1. 1.
1. 1. 1.];
if norm(computed(:,:,1)-expected)>%eps then pause, end
if norm(computed(:,:,2)-expected)>%eps then pause, end
if norm(computed(:,:,3)-expected)>%eps then pause, end
if norm(computed(:,:,4)-expected)>%eps then pause, end
// With one vector
expected=[1. 1. 1.
1. 1. 1.];
computed=ones(expected);
if norm(computed-expected)>%eps then pause, end

