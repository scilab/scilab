getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/quote_data.ref','rb');
%ans = [1 + %i,2]';
if load_ref('%ans') then   pause,end,

%ans = [1 + %i,2].';
if load_ref('%ans') then   pause,end,

x = 'This is a character string';
if load_ref('x') then   pause,end,

%ans = 'He said:''Good''';
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
