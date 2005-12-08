getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/format_data.ref','rb');
x = rand(1, 5);
format('v', 10);x;
if load_ref('x') then   pause,end,

format(20);x;
if load_ref('x') then   pause,end,

format('e', 10);x;
if load_ref('x') then   pause,end,

format(20);x;
if load_ref('x') then   pause,end,


x = [100,%eps];
format('e', 10);x;
if load_ref('x') then   pause,end,

format('v', 10);x;
if load_ref('x') then   pause,end,


%ans = format('v');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
