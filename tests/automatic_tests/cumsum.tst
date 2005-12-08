getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/cumsum_data.ref','rb');
A = [1,2;3,4];
%ans = cumsum(A);
if load_ref('%ans') then   pause,end,

%ans = cumsum(A, 'r');
if load_ref('%ans') then   pause,end,

%ans = cumsum(A, 'c');
if load_ref('%ans') then   pause,end,

a = rand(3, 4) + %i;
[m,n] = size(a);
w = zeros(a);
w(1, eye()) = a(1, :);
for k = 2:m,  w(k, eye()) = w(k - 1, :) + a(k, :);end,%ans = w - cumsum(a, 'r');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
