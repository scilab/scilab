getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/etime_data.ref','rb');
t1 = [2004,6,10,17,0,12.345];
if load_ref('t1') then   pause,end,

t2 = [2004,6,10,17,1,13.965];
if load_ref('t2') then   pause,end,

E1 = etime(t2, t1);
if load_ref('E1') then   pause,end,

t1 = [2004,6,24,162,5,10,17,0,12,345];
if load_ref('t1') then   pause,end,

t2 = [2004,6,24,162,5,10,17,1,13,965];
if load_ref('t2') then   pause,end,

E2 = etime(t2, t1);
if load_ref('E2') then   pause,end,

xdel_run(winsid());

mclose(%U);
