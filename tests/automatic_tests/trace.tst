getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/trace_data.ref','rb');
A = rand(3, 3);
%ans = trace(A) - sum(spec(A));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
