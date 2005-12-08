getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/isempty_data.ref','rb');
a = 1;
if load_ref('a') then   pause,end,

%ans = isempty(a(2:$));
if load_ref('%ans') then   pause,end,

%ans = isempty(find(rand(1:10) == 5));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
