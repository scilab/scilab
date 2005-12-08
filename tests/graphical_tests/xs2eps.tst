getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/xs2eps_data.ref','rb');
%ans = scf(0);
if load_ref('%ans') then   pause,end,

%ans = plot2d();
if load_ref('%ans') then   pause,end,

// Postscript export
filename = 'foo';// ! no extension
%ans = xs2eps(0, filename);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
