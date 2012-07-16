// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK ERROR OUTPUT -->

// <-- Non-regression test for bug 7180 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7180
//
// <-- Short Description -->
// xs2pdf, xs2eps was enable to export a graphic

plot(1:1:5, "r--");
plot(2:1:6, "r-.");
filename = TMPDIR + "/nonreg_test_bug7180.pdf";
xs2pdf(gcf(), filename);
if ~isfile(filename) then pause; end;
filename = TMPDIR + "/nonreg_test_bug7180.eps";
xs2eps(gcf(), filename);
if ~isfile(filename) then pause; end;