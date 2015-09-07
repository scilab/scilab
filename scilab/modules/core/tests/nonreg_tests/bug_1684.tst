//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1684 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1684
//
// <-- Short Description -->
//    I can't compute the following expression:
//    A = 1 + -2;
//    I need to enter A = 1 + (-2);
//    I've got a program which generates automaticaly matlab scripts with this kind of
//    expressions and I don't want to change it to test is the value is negative and
//    then add parenthesis.

a=1+-1;
if a<>0 then pause,end

a=1-+1;
if a<>0 then pause,end

a=1++1;
if a<>2 then pause,end

a=1--1;
if a<>2 then pause,end

a=1*-1;
if a<>-1 then pause,end

a=1*+1;
if a<>1 then pause,end

a=2*+++--2;
if a<>4 then pause,end

a=3+-1*+++--2;
if a<>1 then pause,end
