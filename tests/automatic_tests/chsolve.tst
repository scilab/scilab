getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/chsolve_data.ref','r');
A = sprand(20, 20, 0.1);
A = A * A' + eye();
spcho = chfact(A);
sol = (1:20)';rhs = A * sol;
spcho = chfact(A);
%ans = chsolve(spcho, rhs);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
