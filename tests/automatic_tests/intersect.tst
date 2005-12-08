getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/intersect_data.ref','rb');
A = round(5 * rand(10, 1));
B = round(5 * rand(7, 1));

%ans = intersect(A, B);
if load_ref('%ans') then   pause,end,

[N,ka,kb] = intersect(A, B);
if load_ref('kb') then   pause,end,
if load_ref('ka') then   pause,end,
if load_ref('N') then   pause,end,


%ans = intersect('a' + string(A), 'a' + string(B));
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
