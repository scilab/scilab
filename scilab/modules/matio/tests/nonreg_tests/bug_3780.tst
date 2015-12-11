// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3780 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3780
//
// <-- Short Description -->
// The function loadmatfile does not ignore comments found in ASCII files while loading them, resulting in an error message.
// Moreover, for Matlab compatibility, when this function is called with one input argument load(filename), it must look for filename (as a binary file), filename.mat (as a binary file) or as an ASCII file otherwise.

// Check that comment are ignored

// <-- CLI SHELL MODE -->

loadmatfile("SCI/modules/matio/tests/nonreg_tests/bug_3780.txt", "-ascii");

if ~exists("bug_3780", "local") then pause;end
refvalue = [0,1,2,3,4;
	10,11,12,13,14;
	20,21,22,23,24;
	30,31,32,33,34;
	40,41,42,43,44;
	50,51,52,53,54;
	30,61,62,63,64];
if or(bug_3780<>refvalue) then pause;end

clear bug_3780

// Check that loadmatfile can be call with an ASCII file without -ascii option
loadmatfile("SCI/modules/matio/tests/nonreg_tests/bug_3780.txt");
if or(bug_3780<>refvalue) then pause;end
