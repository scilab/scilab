//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 6516 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6516
//
// <-- Short Description -->
// fullfile did not remove blanks and not create a valid path

ref = pathconvert('SCI/modules/core',%f,%f);
r = fullfile('     SCI     ','      modules    ','        core    ');
if r <> ref then pause,end

ref = pathconvert('a/b/c/d/e/f/g/h/i/j',%f,%f);
r = fullfile('a','b','c','d','e','f','g','h','i','j');
if r <> ref then pause,end

ref = []
r = fullfile([],[]);
if r <> ref then pause,end

ierr = execstr("fullfile()","errcatch");
if ierr <> 10000 then pause,end

ierr = execstr("fullfile(3)","errcatch");
if ierr <> 10000 then pause,end

ierr = execstr("fullfile(''a'',3)","errcatch");
if ierr <> 10000 then pause,end

ierr = execstr("fullfile(''a'',''b'',3)","errcatch");
if ierr <> 10000 then pause,end


