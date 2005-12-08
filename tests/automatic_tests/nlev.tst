getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/nlev_data.ref','rb');
A = rand(3, 3);x = poly(0, 'x');
[NUM,den] = nlev(A, 'x');
if load_ref('den') then   pause,end,
if load_ref('NUM') then   pause,end,

%ans = clean(den - poly(A, 'x'));
if load_ref('%ans') then   pause,end,

%ans = clean(NUM/den - inv(x * eye() - A));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
