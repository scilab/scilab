getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/sub2ind_data.ref','rb');
i = [1,2,1,1,2,1,1];
j = [1,2,3,1,2,3,3];
k = [1,2,1,2,1,2,1];
%ans = sub2ind([2,3,2], i, j, k);
if load_ref('%ans') then   pause,end,


%ans = sub2ind([2,3,2], [i',j',k']);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
