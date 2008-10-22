getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/lex_sort_data.ref','rb');
M = round(2 * rand(20, 3));

%ans = lex_sort(M);
if load_ref('%ans') then   pause,end,

%ans = lex_sort(M, 'unique');
if load_ref('%ans') then   pause,end,

[N,k] = lex_sort(M, [1,3], 'unique');
if load_ref('k') then   pause,end,
if load_ref('N') then   pause,end,


xdel_run(winsid());

mclose(%U);
