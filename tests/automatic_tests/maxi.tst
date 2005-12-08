getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/maxi_data.ref','rb');
[m,n] = maxi([1,3,1]);
if load_ref('n') then   pause,end,
if load_ref('m') then   pause,end,

[m,n] = maxi([3,1,1], [1,3,1], [1,1,3]);
if load_ref('n') then   pause,end,
if load_ref('m') then   pause,end,

[m,n] = maxi([3,-2,1], 1);
if load_ref('n') then   pause,end,
if load_ref('m') then   pause,end,

[m,n] = maxi(list([3,1,1], [1,3,1], [1,1,3]));
if load_ref('n') then   pause,end,
if load_ref('m') then   pause,end,

[m,n] = maxi(list(1, 3, 1));
if load_ref('n') then   pause,end,
if load_ref('m') then   pause,end,

xdel_run(winsid());

mclose(%U);
