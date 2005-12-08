getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/penlaur_data.ref','rb');
F = randpencil([], [1,2], [1,2,3], []);
F = rand(6, 6) * F * rand(6, 6);[E,A] = pen2ea(F);
[Si,Pi,Di] = penlaur(F);
[Bfs,Bis,chis] = glever(F);
%ans = norm(coeff(Bis, 1) - Di, 1);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
