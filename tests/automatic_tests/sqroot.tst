getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/sqroot_data.ref','rb');
X = rand(5, 2) * rand(2, 5);X = X * X';
W = sqroot(X);
if load_ref('W') then   pause,end,

%ans = norm(W * W' - X, 1);
if load_ref('%ans') then   pause,end,

//
X = rand(5, 2) + %i * rand(5, 2);X = X * X';
W = sqroot(X);
if load_ref('W') then   pause,end,

%ans = norm(W * W' - X, 1);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
