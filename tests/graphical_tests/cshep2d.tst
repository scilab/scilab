getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/cshep2d_data.ref','rb');
// interpolation of cos(x)cos(y) with randomly choosen interpolation points
n = 150;// nb of interpolation points
xy = grand(n, 2, 'unf', 0, 2 * %pi);
z = cos(xy(:, 1)) .* cos(xy(:, 2));
xyz = [xy,z];
tl_coef = cshep2d(xyz);

// evaluation on a grid
m = 30;
xx = linspace(0, 2 * %pi, m);
[X,Y] = ndgrid(xx, xx);
Z = eval_cshep2d(X, Y, tl_coef);
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = plot3d(xx, xx, Z, flag=[2,6,4]);
if load_ref('%ans') then   pause,end,

%ans = param3d1(xy(:, 1), xy(:, 2), list(z, -9), flag=[0,0]);
if load_ref('%ans') then   pause,end,

%ans = xtitle('Cubic Shepard Interpolation of cos(x)cos(y) with randomly choosen interpolation points');
if load_ref('%ans') then   pause,end,

%ans = legends('interpolation points', -9, 1);
if load_ref('%ans') then   pause,end,

%ans = xselect();
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
