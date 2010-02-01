// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Digiteo - Pierre Lando
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK ERROR OUTPUT -->

// <-- Non-regression test for bug 4944 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4944
//
// <-- Short Description -->
// If the extension is not provided, some functions adds the extension, some others don't
// 


plot3d();
filename=TMPDIR+"/nonreg_test_bug4944";

xs2png(0,filename); // must save in filename+".png"
if fileinfo(filename+".png") == [] then pause; end;

xs2bmp(0,filename+".bmp"); // must save in filename+".bmp"
if fileinfo(filename+".bmp") == [] then pause; end;

xs2pdf(0,filename);  // must save in filename+".pdf"
if fileinfo(filename+".pdf") == [] then pause; end;

xs2bmp(0,filename+"a.pdf");  // must save in filename+"a.pdf"
if fileinfo(filename+"a.pdf") == [] then pause; end;
