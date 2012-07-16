// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK ERROR OUTPUT -->

// <-- Non-regression test for bug 6718 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6718
//
// <-- Short Description -->
// If a text property in a graphic contains a "<" symbol
// the export returns an error (eps) or ignore the text (svg)

scf(0);
plot(1:10)
a=gca();
a.title.text='a<b';//similar pb with a.x_label.text='a<b'

filename=TMPDIR+"/nonreg_test_bug4944.eps";
xs2eps(0,filename);
if fileinfo(filename) == [] then pause; end;
