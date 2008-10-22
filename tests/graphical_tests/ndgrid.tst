getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/ndgrid_data.ref','rb');
// create a simple 2d grid
nx = 40;ny = 40;
x = linspace(-1, 1, nx);
y = linspace(-1, 1, ny);
[X,Y] = ndgrid(x, y);
// compute a function on the grid and plot it
//deff("z=f(x,y)","z=128*x.^2 .*(1-x).^2 .*y.^2 .*(1-y).^2");
%ans = deff('z=f(x,y)', 'z=x.^2 + y.^3');
if load_ref('%ans') then   pause,end,

Z = f(X, Y);
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

plot3d(x, y, Z, flag=[2,6,4]);%ans = xselect();
if load_ref('%ans') then   pause,end,


// create a simple 3d grid
nx = 10;ny = 6;nz = 4;
x = linspace(0, 2, nx);
y = linspace(0, 1, ny);
z = linspace(0, 0.5, nz);
[X,Y,Z] = ndgrid(x, y, z);
// try to display this 3d grid ...
XF = [];YF = [];ZF = [];
for k = 1:nz,
  [xf,yf,zf] = nf3d(X(:, :, k), Y(:, :, k), Z(:, :, k));
  XF = [XF,xf];YF = [YF,yf];ZF = [ZF,zf];
end,
for j = 1:ny,
  [xf,yf,zf] = nf3d(matrix(X(:, j, :), [nx,nz]), matrix(Y(:, j, :), [nx,nz]), matrix(Z(:, j, :), [nx,nz]));
  XF = [XF,xf];YF = [YF,yf];ZF = [ZF,zf];
end,
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = plot3d(XF, YF, ZF, flag=[0,6,3], leg='X@Y@Z');
if load_ref('%ans') then   pause,end,

xtitle('A 3d grid !');%ans = xselect();
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
