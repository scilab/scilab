getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/trace_data.ref','r');
A = rand(3, 3);
%ans = trace(A) - sum(spec(A));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
