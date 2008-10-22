getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/xgraduate_data.ref','rb');
[x1,xa,np1,np2,kMinr,kMaxr,ar] = xgraduate(-0.3, 0.2);
if load_ref('ar') then   pause,end,
if load_ref('kMaxr') then   pause,end,
if load_ref('kMinr') then   pause,end,
if load_ref('np2') then   pause,end,
if load_ref('np1') then   pause,end,
if load_ref('xa') then   pause,end,
if load_ref('x1') then   pause,end,

xdel_run(winsid());

mclose(%U);
