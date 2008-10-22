getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/randpencil_data.ref','rb');
F = randpencil([0,1], 2, [-1,0,1], 3);
[Q,Z,Qd,Zd,numbeps,numbeta] = kroneck(F);
Qd;
if load_ref('Qd') then   pause,end,
Zd;
if load_ref('Zd') then   pause,end,

s = poly(0, 's');
F = randpencil([], [1,2], s^3 - 2, []);//regular pencil
%ans = det(F);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
