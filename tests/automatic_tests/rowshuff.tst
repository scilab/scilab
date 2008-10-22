getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/rowshuff_data.ref','rb');
F = randpencil([], 2, [1,2,3], []);
F = rand(5, 5) * F * rand(5, 5);// 5 x 5 regular pencil with 3 evals at 1,2,3
[Ws,F1] = rowshuff(F, -1);
[E1,A1] = pen2ea(F1);
%ans = svd(E1);
if load_ref('%ans') then   pause,end,
//E1 non singular
%ans = roots(det(Ws));
if load_ref('%ans') then   pause,end,

%ans = clean(inv(F) - inv(F1) * Ws, 0.0000001);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
