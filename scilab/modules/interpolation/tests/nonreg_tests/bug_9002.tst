//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 9002 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9002
//
// <-- Short Description -->
// linear_interpn, splin2d/interp2d, ... functions ignored imaginary part of input arguments


// test with bsplin3val  
//
deff("v=f(x,y,z)","v=cos(x).*sin(y).*cos(z)");
deff("v=fx(x,y,z)","v=-sin(x).*sin(y).*cos(z)");
deff("v=fxy(x,y,z)","v=-sin(x).*cos(y).*cos(z)");
deff("v=fxyz(x,y,z)","v=sin(x).*cos(y).*sin(z)");
deff("v=fxxyz(x,y,z)","v=cos(x).*cos(y).*sin(z)");
n = 20;
x = linspace(0,2*%pi,n);
y = x;
z = x;
[X,Y,Z] = ndgrid(x,y,z);
V = f(X,Y,Z);
tl = splin3d(x,y,z,V,[5 5 5]);
xp = grand(1,1,"unf",0,2*%pi); 
yp = grand(1,1,"unf",0,2*%pi); 
zp = grand(1,1,"unf",0,2*%pi); 

f_i = bsplin3val(xp, yp, zp, tl, [0 0 0]);

yp = yp + %i;
msgerr = msprintf(gettext("%s: Wrong type for argument #%d: Real matrix expected.\n"), "bsplin3val", 2);
assert_checkerror ("f_i = bsplin3val(xp, yp, zp, tl, [0 0 0]);", msgerr);

// test with cshep2d
//
n = 150;
xy = grand(n,2,"unf",0,2*%pi);
z = cos(xy(:,1)).*cos(xy(:,2));
xyz = [xy z];
tl_coef = cshep2d(xyz);

xyz = [xy z] + %i;
msgerr = msprintf(gettext("%s: Wrong type for argument #%d: Real matrix expected.\n"), "cshep2d", 1);
assert_checkerror ("tl_coef = cshep2d(xyz);", msgerr);

// test with eval_cshep2d
//
clear f;
deff("z=f(x,y)","z = 1+ 50*(x.*(1-x).*y.*(1-y)).^2")
x = linspace(0,1,10);
[X,Y] = ndgrid(x,x);
X = X(:); Y = Y(:); Z = f(X,Y);
S = cshep2d([X Y Z]);
m = 40;
xx = linspace(-1.5,0.5,m);
[xp, yp] = ndgrid(xx,xx);
zp = eval_cshep2d(xp, yp, S);

yp = yp + %i;
msgerr = msprintf(gettext("%s: Wrong type for argument #%d: Real matrix expected.\n"), "eval_cshep2d", 2);
assert_checkerror ("eval_cshep2d(xp, yp, S)", msgerr);

// test with interp
//
x = linspace(0,1,11)';
y = cosh(x-0.5);
d = splin(x,y);
xx = linspace(-0.5,1.5,401)';
yy0 = interp(xx,x,y,d,"C0");

y = y + %i;
msgerr = msprintf(gettext("%s: Wrong type for argument #%d: Real matrix expected.\n"), "interp", 3);
assert_checkerror ("yy0 = interp(xx,x,y,d,''C0'')", msgerr);


// test with interp2d
//
n = 7;
x = linspace(0,2*%pi,n);
y = x;
z = cos(x')*cos(y);
C = splin2d(x, y, z, "periodic");
m = 80;
xx = linspace(-0.5*%pi,2.5*%pi,m);
yy = xx;
[XX,YY] = ndgrid(xx,yy);
zz1 = interp2d(XX,YY, x, y, C, "C0");

YY = YY + %i;
msgerr = msprintf(gettext("%s: Wrong type for argument #%d: Real matrix expected.\n"), "interp2d", 2);
assert_checkerror ("zz1 = interp2d(XX,YY, x, y, C, ''C0'')", msgerr);

// test with interp3d
//
m = 10000;
xp = grand(m,1,"def");
yp = grand(m,1,"def");
zp = grand(m,1,"def");
vp_interp = interp3d(xp,yp,zp, tl);

zp = zp + %i;
msgerr = msprintf(gettext("%s: Wrong type for argument #%d: Real matrix expected.\n"), "interp3d", 3);
assert_checkerror ("vp_interp = interp3d(xp,yp,zp, tl)", msgerr);

// test with linear_interpn
//
x = linspace(0,2*%pi,11);
y = sin(x);
xx = linspace(-2*%pi,4*%pi,400)';
yy = linear_interpn(xx, x, y, "periodic");

xx = xx + %i;
msgerr = msprintf(gettext("%s: Wrong type for argument #%d: Real matrix expected.\n"), "linear_interpn", 1);
assert_checkerror ("yy = linear_interpn(xx, x, y, ''periodic'')", msgerr);

// test with splin2d
//
n = 7;
x = linspace(0, 2 * %pi, n);
y = x;
z = cos(x') * cos(y);
C = splin2d(x, y, z, "periodic");

z = z + %i;
msgerr = msprintf(gettext("%s: Wrong type for argument #%d: Real matrix expected.\n"), "splin2d", 3);
assert_checkerror ("C = splin2d(x, y, z, ''periodic'')", msgerr);

// test with splin2d
//
func =  "v=cos(2*%pi*x).*sin(2*%pi*y).*cos(2*%pi*z)";
clear f;
deff("v = f(x,y,z)",func);
n = 10;  // n x n x n  interpolation points
x = linspace(0,1,n); y=x; z=x; // interpolation grid
[X,Y,Z] = ndgrid(x,y,z);
V = f(X,Y,Z);
tl = splin3d(x,y,z,V,[5 5 5]);

x = x + %i;
msgerr = msprintf(gettext("%s: Wrong type for argument #%d: Real matrix expected.\n"), "splin3d", 1);
assert_checkerror ("tl = splin3d(x,y,z,V,[5 5 5])", msgerr);

// test with splin2d
//
deff("y=runge(x)","y=1 ./(1 + x.^2)")
a = -5;
b = 5;
n = 11;
m = 400;
x = linspace(a, b, n)';
y = runge(x);
d = splin(x, y);

y = y + %i;
msgerr = msprintf(gettext("%s: Wrong type for argument #%d: Real matrix expected.\n"), "splin", 2);
assert_checkerror ("d = splin(x, y);", msgerr);
