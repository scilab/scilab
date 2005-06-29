getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/givens_data.ref','r');
A = [3,4;5,6];
U = givens(A(:, 1));
%ans = U * A;
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
