getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/tokens_data.ref','rb');
%ans = tokens('This is a character string');
if load_ref('%ans') then   pause,end,


%ans = tokens('SCI/demos/scicos', '/');
if load_ref('%ans') then   pause,end,


%ans = tokens('y=a+b*2', ['=','+','*']);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
