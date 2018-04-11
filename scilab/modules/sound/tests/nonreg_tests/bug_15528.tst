// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15528 -->
//
// <-- Bugzilla URL -->
// https://bugzilla.scilab.org/show_bug.cgi?id=15528
//
// <-- Short Description -->
// savewave  write null signal if 24 bits resolution and more than 2 channels

s=[sin([1:0.1:100]);sin([1:0.1:100]);sin([1:0.1:100])];
wavwrite(s,22500,24,"B.wav");
b=wavread("B.wav");
assert_checkalmostequal(b,s,1e-4,[],'element')
