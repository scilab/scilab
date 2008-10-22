getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/scf_data.ref','rb');
f4 = scf(4);//creates figure with id==4 and make it the current one
f0 = scf(0);//creates figure with id==0 and make it the current one
%ans = plot2d();
if load_ref('%ans') then   pause,end,
//draw in current figure (id=0)
scf(f4);// set first created figure as current one
%ans = plot3d();
if load_ref('%ans') then   pause,end,
//draw in current figure (id=4)
xdel_run(winsid());

mclose(%U);
