// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 7994 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7994
//
// <-- Short Description -->
// xs2emf created empty files on Windows

emfFilename = TMPDIR + "/bug_7994.emf";
plot3d();
f=gcf();

xs2emf(f.figure_id, emfFilename);
if ~isfile(emfFilename) then pause, end
info = fileinfo(emfFilename);
if info(1) < 592080 then pause, end

