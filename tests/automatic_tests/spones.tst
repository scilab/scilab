getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/spones_data.ref','rb');
A = sprand(10, 12, 0.1);
sp = spones(A);
if load_ref('sp') then   pause,end,

B = A ~= 0;
if load_ref('B') then   pause,end,

%ans = bool2s(B);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
