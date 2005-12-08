getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/rankqr_data.ref','rb');
A = rand(5, 3) * rand(3, 7);
[Q,R,JPVT,RANK,SVAL] = rankqr(A, %eps);
if load_ref('SVAL') then   pause,end,
if load_ref('RANK') then   pause,end,
if load_ref('JPVT') then   pause,end,
if load_ref('R') then   pause,end,
if load_ref('Q') then   pause,end,

xdel_run(winsid());

mclose(%U);
