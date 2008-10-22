getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/ereduc_data.ref','rb');
X = [1,2,3;4,5,6];
if load_ref('X') then   pause,end,

[E,Q,Z,stair,rk] = ereduc(X, 1.00000000000E-15);
if load_ref('rk') then   pause,end,
if load_ref('stair') then   pause,end,
if load_ref('Z') then   pause,end,
if load_ref('Q') then   pause,end,
if load_ref('E') then   pause,end,

xdel_run(winsid());

mclose(%U);
