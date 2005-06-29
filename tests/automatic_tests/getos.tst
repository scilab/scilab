getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/getos_data.ref','r');
OS = getos();
if load_ref('OS') then   pause,end,

[OS,version] = getos();
if load_ref('version') then   pause,end,
if load_ref('OS') then   pause,end,

xdel_run(winsid());

mclose(%U);
