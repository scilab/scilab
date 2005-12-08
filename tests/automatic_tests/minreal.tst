getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/minreal_data.ref','rb');
A = [-eye(2, 2),rand(2, 2);zeros(2, 2),-2 * eye(2, 2)];
B = [rand(2, 2);zeros(2, 2)];C = rand(2, 4);
sl = syslin('c', A, B, C);
slb = minreal(sl);
%ans = ss2tf(sl);
if load_ref('%ans') then   pause,end,

%ans = ss2tf(slb);
if load_ref('%ans') then   pause,end,

%ans = ctr_gram(sl);
if load_ref('%ans') then   pause,end,

%ans = clean(ctr_gram(slb));
if load_ref('%ans') then   pause,end,

%ans = clean(obs_gram(slb));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
