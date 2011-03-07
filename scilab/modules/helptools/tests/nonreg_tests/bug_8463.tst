// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- JVM MANDATORY -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 7681 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7681
//
// <-- Short Description -->
// scilab pdfs (fr_FR and pt_BR) were not fully generated.

r1 = xmltopdf([], [], "fr_FR");
info1 = fileinfo(r1);
if info1(1) < 10000000 then pause, end

r2 = xmltopdf([], [], "pt_BR");
info2 = fileinfo(r2);
if info2(1) < 10000000 then pause, end

// opens files and check that they are VALID !!! 



