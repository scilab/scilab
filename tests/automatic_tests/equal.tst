getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/equal_data.ref','rb');
a = sin(3.2);
if load_ref('a') then   pause,end,

[u,s] = schur(rand(3, 3));
if load_ref('s') then   pause,end,
if load_ref('u') then   pause,end,

%ans = (1:10) == 4;
if load_ref('%ans') then   pause,end,

%ans = 1 ~= 2;
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
