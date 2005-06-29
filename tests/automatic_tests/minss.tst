getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/minss_data.ref','r');
sl = syslin('c', [1,0;0,2], [1;0], [2,1]);
ssprint(sl);
%ans = ssprint(minss(sl));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
