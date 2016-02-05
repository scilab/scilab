// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- NO CHECK ERROR OUTPUT -->
//
// <-- Non-regression test for bug 7640 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7640
//
// <-- Short Description -->
// xs2pdf, xs2eps, xs2emf crashed if filename prefix had less three characters.

cd TMPDIR;
s = scf(0);

t=0:0.1:10;
plot(t,sin(t));

// eps
xs2eps(0, "t.eps");
if ~isfile("t.eps") then pause, end
info = fileinfo("t.eps");
if info(1) == 0 then pause, end

// eps
xs2eps(0, "t1.eps");
if ~isfile("t1.eps") then pause, end
info = fileinfo("t1.eps");
if info(1) == 0 then pause, end

//pdf
xs2pdf(0, "t1.pdf");
if ~isfile("t1.pdf") then pause, end
info = fileinfo("t1.pdf");
if info(1) == 0 then pause, end

//emf
xs2emf(0, "t1.emf");
if ~isfile("t1.emf") then pause, end
info = fileinfo("t1.emf");
if info(1) == 0 then pause, end
