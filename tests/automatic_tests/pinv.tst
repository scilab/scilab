getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/pinv_data.ref','r');
A = rand(5, 2) * rand(2, 4);
%ans = norm(A * pinv(A) * A - A, 1);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
