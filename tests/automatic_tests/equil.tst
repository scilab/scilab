getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/equil_data.ref','rb');
P = rand(4, 4);P = P * P';
Q = rand(4, 4);Q = Q * Q';
T = equil(P, Q);
if load_ref('T') then   pause,end,

%ans = clean(T * P * T');
if load_ref('%ans') then   pause,end,

%ans = clean(inv(T)' * Q * inv(T));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
