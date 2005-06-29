getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/stripblanks_data.ref','r');
a = '  123   ';
%ans = '!' + a + '!';
if load_ref('%ans') then   pause,end,

%ans = '!' + stripblanks(a) + '!';
if load_ref('%ans') then   pause,end,

a = ['  123   ',' xyz'];
if load_ref('a') then   pause,end,

%ans = strcat(stripblanks(a));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
