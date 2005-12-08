getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/kron_data.ref','rb');
A = [1,2;3,4];
%ans = kron(A, A);
if load_ref('%ans') then   pause,end,

%ans = A .*. A;
if load_ref('%ans') then   pause,end,

%ans = sparse(A) .*. sparse(A);
if load_ref('%ans') then   pause,end,

A(1, 1) = %i;
%ans = kron(A, A);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
