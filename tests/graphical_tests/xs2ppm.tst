getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/xs2ppm_data.ref','rb');
%ans = scf(0);
if load_ref('%ans') then   pause,end,

%ans = plot2d();
if load_ref('%ans') then   pause,end,

%ans = xs2ppm(0, 'foo.ppm');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
