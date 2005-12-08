getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/splin3d_data.ref','rb');
// example 1
func = 'v=cos(2*%pi*x).*sin(2*%pi*y).*cos(2*%pi*z)';
deff('v=f(x,y,z)', func);
n = 10;// n x n x n  interpolation points
x = linspace(0, 1, n);y = x;z = x;// interpolation grid
[X,Y,Z] = ndgrid(x, y, z);
V = f(X, Y, Z);
tl = splin3d(x, y, z, V, [5,5,5]);
m = 10000;
// compute an approximated error
xp = grand(m, 1, 'def');yp = grand(m, 1, 'def');zp = grand(m, 1, 'def');
vp_exact = f(xp, yp, zp);
vp_interp = interp3d(xp, yp, zp, tl);
er = max(abs(vp_exact - vp_interp));
if load_ref('er') then   pause,end,

// now retry with n=20 and see the error


// example 2 (see linear_interpn help page which have the
//            same example with trilinear interpolation)
%ans = getf('SCI/demos/interp/interp_demo.sci');
if load_ref('%ans') then   pause,end,

func = 'v=(x-0.5).^2 + (y-0.5).^3 + (z-0.5).^2';
deff('v=f(x,y,z)', func);
n = 5;
x = linspace(0, 1, n);y = x;z = x;
[X,Y,Z] = ndgrid(x, y, z);
V = f(X, Y, Z);
tl = splin3d(x, y, z, V);
// compute (and display) the 3d spline interpolant on some slices
m = 41;
dir = ['z=','z=','z=','x=','y='];
val = [0.1,0.5,0.9,0.5,0.5];
ebox = [0,1,0,1,0,1];
XF = [];YF = [];ZF = [];VF = [];
for i = 1:length(val),
  [Xm,Xp,Ym,Yp,Zm,Zp] = slice_parallelepiped(dir(i), val(i), ebox, m, m, m);
  Vm = interp3d(Xm, Ym, Zm, tl);
  [xf,yf,zf,vf] = nf3dq(Xm, Ym, Zm, Vm, 1);
  XF = [XF,xf];YF = [YF,yf];ZF = [ZF,zf];VF = [VF,vf];
  Vp = interp3d(Xp, Yp, Zp, tl);
  [xf,yf,zf,vf] = nf3dq(Xp, Yp, Zp, Vp, 1);
  XF = [XF,xf];YF = [YF,yf];ZF = [ZF,zf];VF = [VF,vf];
end,
nb_col = 128;
vmin = min(VF);vmax = max(VF);
color = dsearch(VF, linspace(vmin, vmax, nb_col + 1));
xset('colormap', jetcolormap(nb_col));
xbasc_run();xset('hidden3d', xget('background'));
%ans = colorbar(vmin, vmax);
if load_ref('%ans') then   pause,end,

%ans = plot3d(XF, YF, list(ZF, color), flag=[-1,6,4]);
if load_ref('%ans') then   pause,end,

%ans = xtitle('3d spline interpolation of ' + func);
if load_ref('%ans') then   pause,end,

%ans = xselect();
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
