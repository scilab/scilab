getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/getversion_data.ref','r');
%ans = getversion();
if load_ref_nocheck('%ans') then   pause,end,

[version,opts] = getversion();
if load_ref_nocheck('opts') then   pause,end,
if load_ref_nocheck('version') then   pause,end,

xdel_run(winsid());

mclose(%U);
