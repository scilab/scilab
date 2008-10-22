getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/colormap_data.ref','rb');
n = 64;
r = linspace(0, 1, n)';
g = linspace(1, 0, n)';
b = ones(r);
cmap = [r,g,b];
f = gcf();f('color_map') = cmap;
%ans = plot3d1();
if load_ref('%ans') then   pause,end,

f('color_map') = get(sdf(), 'color_map');
xdel_run(winsid());

mclose(%U);
