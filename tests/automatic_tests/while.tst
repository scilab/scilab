getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/while_data.ref','r');
e = 1;a = 1;k = 1;
while norm(a - (a + e), 1) > %eps then  e = e/2;k = k + 1;end,
e;
if load_ref('e') then   pause,end,
k;
if load_ref('k') then   pause,end,

xdel_run(winsid());

mclose(%U);
