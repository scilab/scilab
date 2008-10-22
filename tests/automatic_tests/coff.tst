getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/coff_data.ref','rb');
M = [1,2;0,3];
[N,d] = coff(M);
if load_ref('d') then   pause,end,
if load_ref('N') then   pause,end,

%ans = N/d;
if load_ref('%ans') then   pause,end,

%ans = inv(%s * eye() - M);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
