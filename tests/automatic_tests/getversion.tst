getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/getversion_data.ref','r');
%ans = getversion();
if load_ref('%ans') then   pause,end,

[version,opts] = getversion();
if load_ref('opts') then   pause,end,
if load_ref('version') then   pause,end,

xdel_run(winsid());

mclose(%U);
