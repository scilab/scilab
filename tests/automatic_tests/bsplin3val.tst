getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/bsplin3val_data.ref','rb');
deff('v=f(x,y,z)', 'v=cos(x).*sin(y).*cos(z)');
deff('v=fx(x,y,z)', 'v=-sin(x).*sin(y).*cos(z)');
deff('v=fxy(x,y,z)', 'v=-sin(x).*cos(y).*cos(z)');
deff('v=fxyz(x,y,z)', 'v=sin(x).*cos(y).*sin(z)');
deff('v=fxxyz(x,y,z)', 'v=cos(x).*cos(y).*sin(z)');
n = 20;// n x n x n  interpolation points
x = linspace(0, 2 * %pi, n);y = x;z = x;// interpolation grid
[X,Y,Z] = ndgrid(x, y, z);V = f(X, Y, Z);
tl = splin3d(x, y, z, V, [5,5,5]);

// compute f and some derivates on a point
// and compare with the spline interpolant
xp = grand(1, 1, 'unf', 0, 2 * %pi);
yp = grand(1, 1, 'unf', 0, 2 * %pi);
zp = grand(1, 1, 'unf', 0, 2 * %pi);

f_e = f(xp, yp, zp);
if load_ref('f_e') then   pause,end,

f_i = bsplin3val(xp, yp, zp, tl, [0,0,0]);
if load_ref('f_i') then   pause,end,


fx_e = fx(xp, yp, zp);
if load_ref('fx_e') then   pause,end,

fx_i = bsplin3val(xp, yp, zp, tl, [1,0,0]);
if load_ref('fx_i') then   pause,end,


fxy_e = fxy(xp, yp, zp);
if load_ref('fxy_e') then   pause,end,

fxy_i = bsplin3val(xp, yp, zp, tl, [1,1,0]);
if load_ref('fxy_i') then   pause,end,


fxyz_e = fxyz(xp, yp, zp);
if load_ref('fxyz_e') then   pause,end,

fxyz_i = bsplin3val(xp, yp, zp, tl, [1,1,1]);
if load_ref('fxyz_i') then   pause,end,


fxxyz_e = fxxyz(xp, yp, zp);
if load_ref('fxxyz_e') then   pause,end,

fxxyz_i = bsplin3val(xp, yp, zp, tl, [2,1,1]);
if load_ref('fxxyz_i') then   pause,end,


xdel_run(winsid());

mclose(%U);
