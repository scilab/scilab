getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/luget_data.ref','rb');
a = rand(5, 2) * rand(2, 5);A = sparse(a);
[hand,rk] = lufact(A);[P,L,U,Q] = luget(hand);
%ans = full(L);
if load_ref('%ans') then   pause,end,
%ans = P * L * U * Q - A;
if load_ref('%ans') then   pause,end,

%ans = clean(P * L * U * Q - A);
if load_ref('%ans') then   pause,end,

%ans = ludel(hand);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
