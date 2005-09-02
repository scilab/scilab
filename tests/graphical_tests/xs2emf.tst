getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/graphic/xs2emf_data.ref','r');
//simple example
%ans = scf(0);
if load_ref('%ans') then   pause,end,

%ans = plot2d();
if load_ref('%ans') then   pause,end,

%ans = xs2emf(0, 'foo.emf');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
