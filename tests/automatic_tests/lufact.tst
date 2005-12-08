getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/lufact_data.ref','rb');
a = rand(5, 5);b = rand(5, 1);A = sparse(a);
[h,rk] = lufact(A);
x = lusolve(h, b);%ans = a * x - b;
if load_ref('%ans') then   pause,end,

%ans = ludel(h);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
