// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

driver('GIF');
xinit(TMPDIR+'/mygiffile.gif');
plot3d();
xend();
assert_checktrue(isfile(TMPDIR+'/mygiffile.gif'));
a=fileinfo(TMPDIR+'/mygiffile.gif');
assert_checktrue(a(1)>10);

driver('JPG');
xinit(TMPDIR+'/myjpgfile.jpg');
plot3d();
xend();
assert_checktrue(isfile(TMPDIR+'/myjpgfile.jpg'));
a=fileinfo(TMPDIR+'/myjpgfile.jpg');
assert_checktrue(a(1)>10);


driver('JPEG');
xinit(TMPDIR+'/myjpegfile.jpeg');
plot3d();
xend();
assert_checktrue(isfile(TMPDIR+'/myjpegfile.jpeg'));
a=fileinfo(TMPDIR+'/myjpegfile.jpeg');
assert_checktrue(a(1)>10);

driver('PDF');
xinit(TMPDIR+'/mypdffile.pdf');
plot3d();
xend();
assert_checktrue(isfile(TMPDIR+'/mypdffile.pdf'));
a=fileinfo(TMPDIR+'/mypdffile.pdf');
assert_checktrue(a(1)>10);

driver('PS');
xinit(TMPDIR+'/mypsfile.ps');
plot3d();
xend();
assert_checktrue(isfile(TMPDIR+'/mypsfile.ps'));
a=fileinfo(TMPDIR+'/mypsfile.ps');
assert_checktrue(a(1)>10);

driver('BMP');
xinit(TMPDIR+'/mybmpfile.bmp');
plot3d();
xend();
assert_checktrue(isfile(TMPDIR+'/mybmpfile.bmp'));
a=fileinfo(TMPDIR+'/mybmpfile.bmp');
assert_checktrue(a(1)>10);

driver('PPM');
xinit(TMPDIR+'/myppmfile.ppm');
plot3d();
xend();
assert_checktrue(isfile(TMPDIR+'/myppmfile.ppm'));
a=fileinfo(TMPDIR+'/myppmfile.ppm');
assert_checktrue(a(1)>10);
