getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/samplef_data.ref','rb');
a = [3,7,9;22,4,2];
if load_ref('a') then   pause,end,

f1 = [10,1,1,1,1,1];
if load_ref('f1') then   pause,end,

f2 = [1;15];
if load_ref('f2') then   pause,end,

f3 = [10,1,1];
if load_ref('f3') then   pause,end,

s = samplef(15, a, f1);
if load_ref('s') then   pause,end,

s = samplef(15, a, f2, 'r');
if load_ref('s') then   pause,end,

s = samplef(15, a, f3, 'c');
if load_ref('s') then   pause,end,


xdel_run(winsid());

mclose(%U);
