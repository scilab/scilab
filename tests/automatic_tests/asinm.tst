getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/asinm_data.ref','rb');
A = [1,2;3,4];
if load_ref('A') then   pause,end,

%ans = sinm(asinm(A));
if load_ref('%ans') then   pause,end,

%ans = asinm(A) + %i * logm(%i * A + sqrtm(eye() - A * A));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
