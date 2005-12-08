getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/xs2emf_data.ref','rb');
//simple example
%ans = scf(0);
if load_ref('%ans') then   pause,end,

%ans = plot2d();
if load_ref('%ans') then   pause,end,

%ans = xs2emf(0, 'foo.emf');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
