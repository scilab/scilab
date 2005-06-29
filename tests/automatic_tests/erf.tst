getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/erf_data.ref','r');
deff('y=f(t)', 'y=exp(-t^2)');
%ans = erf(0.5) - 2/sqrt(%pi) * intg(0, 0.5, f);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
