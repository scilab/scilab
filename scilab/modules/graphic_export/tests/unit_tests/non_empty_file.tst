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
xs2bmp(figIndex, "textExport.bmp")
fileSize = fileinfo("textExport.bmp");

// check that file size is greater than 10K
minimumSize = 10000
if (fileSize(1) < minimumSize) then pause; end;
mdelete("textExport.bmp");

// same for other formats
xs2gif(figIndex, "textExport.gif")
fileSize = fileinfo("textExport.gif");
if (fileSize(1) < minimumSize) then pause; end;
mdelete("textExport.gif");

xs2jpg(figIndex, "textExport.jpg")
fileSize = fileinfo("textExport.jpg");
if (fileSize(1) < minimumSize) then pause; end;
mdelete("textExport.jpg");

xs2png(figIndex, "textExport.png")
fileSize = fileinfo("textExport.png");
if (fileSize(1) < minimumSize) then pause; end;
mdelete("textExport.png");

// vectorial export
// increase minimum size
minimumSize = 10000

xs2ps(figIndex, "textExport.ps")
fileSize = fileinfo("textExport.ps");
if (fileSize(1) < minimumSize) then pause; end;
mdelete("textExport.ps");

xs2eps(figIndex, "textExport.eps")
fileSize = fileinfo("textExport.eps");
if (fileSize(1) < minimumSize) then pause; end;
mdelete("textExport.eps");

xs2svg(figIndex, "textExport.svg")
fileSize = fileinfo("textExport.svg");
if (fileSize(1) < minimumSize) then pause; end;
mdelete("textExport.svg");

xs2pdf(figIndex, "textExport.pdf")
fileSize = fileinfo("textExport.pdf");
if (fileSize(1) < minimumSize) then pause; end;
mdelete("textExport.pdf");

