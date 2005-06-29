getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/fullrfk_data.ref','r');
A = rand(5, 2) * rand(2, 5);[Bk,Ck] = fullrfk(A, 3);
%ans = norm(Bk * Ck - (A^3), 1);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
