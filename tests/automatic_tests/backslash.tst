getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/backslash_data.ref','rb');
A = rand(3, 2);b = [1;1;1];x = A\b;y = pinv(A) * b;%ans = x - y;
if load_ref('%ans') then   pause,end,

A = rand(2, 3);b = [1;1];x = A\b;y = pinv(A) * b;%ans = x - y;
if load_ref('%ans') then   pause,end,
%ans = A * x - b;
if load_ref('%ans') then   pause,end,
%ans = A * y - b;
if load_ref('%ans') then   pause,end,

A = rand(3, 1) * rand(1, 2);b = [1;1;1];x = A\b;y = pinv(A) * b;%ans = A * x - b;
if load_ref('%ans') then   pause,end,
%ans = A * y - b;
if load_ref('%ans') then   pause,end,

A = rand(2, 1) * rand(1, 3);b = [1;1];x = A\b;y = pinv(A) * b;%ans = A * x - b;
if load_ref('%ans') then   pause,end,
%ans = A * y - b;
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
