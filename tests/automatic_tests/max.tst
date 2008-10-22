getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/max_data.ref','rb');
[m,n] = max([1,3,1]);
if load_ref('n') then   pause,end,
if load_ref('m') then   pause,end,

[m,n] = max([3,1,1], [1,3,1], [1,1,3]);
if load_ref('n') then   pause,end,
if load_ref('m') then   pause,end,

[m,n] = max([3,-2,1], 1);
if load_ref('n') then   pause,end,
if load_ref('m') then   pause,end,

[m,n] = max(list([3,1,1], [1,3,1], [1,1,3]));
if load_ref('n') then   pause,end,
if load_ref('m') then   pause,end,

[m,n] = max(list(1, 3, 1));
if load_ref('n') then   pause,end,
if load_ref('m') then   pause,end,

xdel_run(winsid());

mclose(%U);
