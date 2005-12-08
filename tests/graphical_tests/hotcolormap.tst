getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/hotcolormap_data.ref','rb');
%ans = xset('colormap', hotcolormap(32));
if load_ref('%ans') then   pause,end,

%ans = plot3d1();
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
