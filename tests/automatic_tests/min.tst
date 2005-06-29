getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/min_data.ref','r');
[m,n] = min([1,3,1]);
if load_ref('n') then   pause,end,
if load_ref('m') then   pause,end,

[m,n] = min([3,1,1], [1,3,1], [1,1,3]);
if load_ref('n') then   pause,end,
if load_ref('m') then   pause,end,

[m,n] = min(list([3,1,1], [1,3,1], [1,1,3]));
if load_ref('n') then   pause,end,
if load_ref('m') then   pause,end,

[m,n] = min(list(1, 3, 1));
if load_ref('n') then   pause,end,
if load_ref('m') then   pause,end,

xdel_run(winsid());

mclose(%U);
