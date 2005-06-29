getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/unique_data.ref','r');
M = round(2 * rand(20, 1));

%ans = unique(M);
if load_ref('%ans') then   pause,end,

[N,k] = unique(M);
if load_ref('k') then   pause,end,
if load_ref('N') then   pause,end,


%ans = unique(string(M));
if load_ref('%ans') then   pause,end,

[N,k] = unique(string(M));
if load_ref('k') then   pause,end,
if load_ref('N') then   pause,end,


xdel_run(winsid());

mclose(%U);
