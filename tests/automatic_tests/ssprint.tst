getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/ssprint_data.ref','rb');
a = [1,1;0,1];b = [0,1;1,0];c = [1,1];d = [3,2];
%ans = ssprint(syslin('c', a, b, c, d));
if load_ref('%ans') then   pause,end,

%ans = ssprint(syslin('d', a, b, c, d));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
