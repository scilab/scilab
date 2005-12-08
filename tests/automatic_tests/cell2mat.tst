getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/cell2mat_data.ref','rb');

c = makecell([2,2], [1,2,3;6,7,8], [4,5;9,10], [11,12;16,17], [14,13,15;18,19,20]);
if load_ref('c') then   pause,end,

%ans = cell2mat(c);
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
