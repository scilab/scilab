getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/kernel_data.ref','rb');
A = rand(3, 1) * rand(1, 3);
%ans = A * kernel(A);
if load_ref('%ans') then   pause,end,

A = sparse(A);
%ans = clean(A * kernel(A));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
