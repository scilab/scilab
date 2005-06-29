getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/unobs_data.ref','r');
A = diag([1,2,3]);C = [1,0,0];
%ans = unobs(A, C);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
