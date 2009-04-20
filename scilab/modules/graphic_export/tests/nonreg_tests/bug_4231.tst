// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 4231 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4231
//
// <-- Short Description -->
// plot();xs2png(0,TMPDIR+filesep()+'toto.png') crashs !!!
// 


plot();
ierr = execstr('xs2png(0,TMPDIR+filesep()+''bug_4231.png'');','errcatch');
if fileinfo(TMPDIR+filesep()+'bug_4231.png') == [] then pause,end

