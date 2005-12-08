getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/ind2sub_data.ref','rb');
%ans = ind2sub([2,3,2], 1:12);
if load_ref('%ans') then   pause,end,

[i,j,k] = ind2sub([2,3,2], 1:12);
if load_ref('k') then   pause,end,
if load_ref('j') then   pause,end,
if load_ref('i') then   pause,end,

xdel_run(winsid());

mclose(%U);
