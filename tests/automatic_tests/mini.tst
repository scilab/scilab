getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/mini_data.ref','rb');
[m,n] = mini([1,3,1]);
if load_ref('n') then   pause,end,
if load_ref('m') then   pause,end,

[m,n] = mini([3,1,1], [1,3,1], [1,1,3]);
if load_ref('n') then   pause,end,
if load_ref('m') then   pause,end,

[m,n] = mini(list([3,1,1], [1,3,1], [1,1,3]));
if load_ref('n') then   pause,end,
if load_ref('m') then   pause,end,

[m,n] = mini(list(1, 3, 1));
if load_ref('n') then   pause,end,
if load_ref('m') then   pause,end,

xdel_run(winsid());

mclose(%U);
