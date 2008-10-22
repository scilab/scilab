getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/string_data.ref','rb');
%ans = string(rand(2, 2));
if load_ref('%ans') then   pause,end,

%ans = deff('y=mymacro(x)', 'y=x+1');
if load_ref('%ans') then   pause,end,

[out,in,text] = string(mymacro);
if load_ref('text') then   pause,end,
if load_ref('in') then   pause,end,
if load_ref('out') then   pause,end,

x = 123.356;%ans = 'Result is ' + string(x);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
