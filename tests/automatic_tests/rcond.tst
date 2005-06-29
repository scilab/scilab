getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/rcond_data.ref','r');
A = diag(1:10);
%ans = rcond(A);
if load_ref('%ans') then   pause,end,

A(1, 1) = 0.000001;
%ans = rcond(A);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
