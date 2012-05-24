// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 5466 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5466
//
// <-- Short Description -->
// I met some problems while saving scilab variables in a Matlab data file. 
// Some times, the variables are not written in the data file.

A = sprand(400,400,0.01);
B = sprand(100,100,0.1);
savematfile(TMPDIR + filesep() + "bug_5466.mat","A","B");
clear
loadmatfile(TMPDIR + filesep() + "bug_5466.mat","A","B");

if or(size(A)<>[400 400]) then pause;end
if or(size(B)<>[100 100]) then pause;end

clear
A = rand(10,10);
savematfile(TMPDIR + filesep() + "bug_5466.mat","A");
clear
loadmatfile(TMPDIR + filesep() + "bug_5466.mat","A");
if or(size(A)<>[10 10]) then pause;end
