getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/addf_data.ref','rb');
%ans = addf('0', '1');
if load_ref('%ans') then   pause,end,

%ans = addf('1', 'a');
if load_ref('%ans') then   pause,end,

%ans = addf('1', '2');
if load_ref('%ans') then   pause,end,

%ans = 'a' + 'b';
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
