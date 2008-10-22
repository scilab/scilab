getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/xs2bmp_data.ref','rb');
//simple example
%ans = scf(0);
if load_ref('%ans') then   pause,end,

%ans = plot2d();
if load_ref('%ans') then   pause,end,

%ans = xs2bmp(0, 'foo.bmp');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
