getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/graphic/graycolormap_data.ref','r');
%ans = xset('colormap', graycolormap(32));
if load_ref('%ans') then   pause,end,

%ans = plot3d1();
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
