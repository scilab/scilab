getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/cellstr_data.ref','r');

%ans = cellstr('foo');
if load_ref('%ans') then   pause,end,

%ans = cellstr(['sci','lab']);
if load_ref('%ans') then   pause,end,

%ans = cellstr(['abc','def','gh';'i','j','klm']);
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
