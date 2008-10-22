// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Jean-Baptiste SILVY <jean-baptiste.silvy@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// test to check that generated files are not empty

// create some data to export
scf();
plot2d;
plot3d;

// try all kinds of exports
fig = gcf();
figIndex =fig.figure_id;
xs2bmp(figIndex, TMPDIR+"/textExport.bmp")
fileSize = fileinfo(TMPDIR+"/textExport.bmp");

// check that file size is greater than 10K
minimumSize = 10000;
if (fileSize(1) < minimumSize) then pause; end;
mdelete(TMPDIR+"/textExport.bmp");

// same for other formats
xs2gif(figIndex, TMPDIR+"/textExport.gif")
fileSize = fileinfo(TMPDIR+"/textExport.gif");
if (fileSize(1) < minimumSize) then pause; end;
mdelete(TMPDIR+"/textExport.gif");

xs2jpg(figIndex, TMPDIR+"/textExport.jpg")
fileSize = fileinfo(TMPDIR+"/textExport.jpg");
if (fileSize(1) < minimumSize) then pause; end;
mdelete(TMPDIR+"/textExport.jpg");

xs2png(figIndex, TMPDIR+"/textExport.png")
fileSize = fileinfo(TMPDIR+"/textExport.png");
if (fileSize(1) < minimumSize) then pause; end;
mdelete(TMPDIR+"/textExport.png");

// vectorial export
// increase minimum size
minimumSize = 10000;

xs2ps(figIndex, TMPDIR+"/textExport.ps")
fileSize = fileinfo(TMPDIR+"/textExport.ps");
if (fileSize(1) < minimumSize) then pause; end;
mdelete(TMPDIR+"/textExport.ps");

xs2eps(figIndex, TMPDIR+"/textExport.eps")
fileSize = fileinfo(TMPDIR+"/textExport.eps");
if (fileSize(1) < minimumSize) then pause; end;
mdelete(TMPDIR+"/textExport.eps");

xs2svg(figIndex, TMPDIR+"/textExport.svg")
fileSize = fileinfo(TMPDIR+"/textExport.svg");
if (fileSize(1) < minimumSize) then pause; end;
mdelete(TMPDIR+"/textExport.svg");

xs2pdf(figIndex, TMPDIR+"/textExport.pdf")
fileSize = fileinfo(TMPDIR+"/textExport.pdf");
if (fileSize(1) < minimumSize) then pause; end;
mdelete(TMPDIR+"/textExport.pdf");

