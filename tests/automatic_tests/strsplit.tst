getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/strsplit_data.ref','rb');
S = 'strsplit splits a string into a vector of strings';
%ans = strsplit(S, [15,25,30]);
if load_ref('%ans') then   pause,end,

ind = strindex(S, ' ');
if load_ref('ind') then   pause,end,

xdel_run(winsid());

mclose(%U);
