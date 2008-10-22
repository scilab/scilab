getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/clf_data.ref','rb');
f4 = scf(4);//creates figure with id==4 and make it the current one
f4('color_map') = jetcolormap(64);
f4('figure_size') = [400,200];
%ans = plot2d();
if load_ref('%ans') then   pause,end,

%ans = clf_run(f4, 'reset');
if load_ref('%ans') then   pause,end,

f0 = scf(0);//creates figure with id==0 and make it the current one
f0('color_map') = hotcolormap(128);
plot3d1();
%ans = clf_run();
if load_ref('%ans') then   pause,end,
// equivalent to clf(gcf(),'clear')
plot3d1();// color_map unchanged
%ans = clf_run(gcf(), 'reset');
if load_ref('%ans') then   pause,end,

plot3d1();// color_map changed (back to the default one with 32 colors)

xdel_run(winsid());

mclose(%U);
