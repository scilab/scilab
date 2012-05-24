// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 4439 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4439
//
// <-- Short Description -->
//    Unexpected complex conjugates when using cat

A = 1+%i;
B = 2-%i;

if ~and(cat(1,A,B)==[A;B]) then pause; end
