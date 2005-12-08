getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/stripblanks_data.ref','rb');
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
