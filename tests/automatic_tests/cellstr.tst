getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/cellstr_data.ref','rb');

%ans = cellstr('foo');
if load_ref('%ans') then   pause,end,

%ans = cellstr(['sci','lab']);
if load_ref('%ans') then   pause,end,

%ans = cellstr(['abc','def','gh';'i','j','klm']);
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
