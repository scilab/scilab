getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/colcomp_data.ref','rb');
A = rand(5, 2) * rand(2, 5);
[X,r] = colcomp(A);
%ans = norm(A * X(:, 1:$ - r), 1);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
