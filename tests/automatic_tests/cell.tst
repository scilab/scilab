getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/cell_data.ref','rb');

%ans = cell(3);
if load_ref('%ans') then   pause,end,

%ans = cell(3, 3);
if load_ref('%ans') then   pause,end,

%ans = cell([2,3,4]);
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
