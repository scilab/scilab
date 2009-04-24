// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3666 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3666
//
// <-- Short Description -->
//    char encoding with length and input

// <-- INTERACTIVE TEST -->

// -- 1 --

S = 'ôpéra';
if length(S) <> 5 then pause,end

// -- 2 --
x = input('Your name ?', 'string')

// Please type : Gaëlle
// and check that x == Gaëlle
if x <> 'Gaëlle' then pause,end

