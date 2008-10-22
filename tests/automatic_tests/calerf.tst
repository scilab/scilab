getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/calerf_data.ref','rb');
deff('y=f(t)', 'y=exp(-t^2)');
%ans = calerf(1, 0);
if load_ref('%ans') then   pause,end,

%ans = 2/sqrt(%pi) * intg(0, 1, f);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
