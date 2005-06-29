getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/cond_data.ref','r');
A = testmatrix('hilb', 6);
%ans = cond(A);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
