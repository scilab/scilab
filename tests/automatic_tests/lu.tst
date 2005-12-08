getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/lu_data.ref','rb');
a = rand(4, 4);
[l,u] = lu(a);
if load_ref('u') then   pause,end,
if load_ref('l') then   pause,end,

%ans = norm(l * u - a);
if load_ref('%ans') then   pause,end,


[h,rk] = lufact(sparse(a));
if load_ref('rk') then   pause,end,
if load_ref('h') then   pause,end,
// lufact fonctionne avec des matrices creuses
[P,L,U,Q] = luget(h);
%ans = ludel(h);
if load_ref('%ans') then   pause,end,

P = full(P);L = full(L);U = full(U);Q = full(Q);
%ans = norm(P * L * U * Q - a);
if load_ref('%ans') then   pause,end,
// P,Q sont des matrices de permutation
xdel_run(winsid());

mclose(%U);
