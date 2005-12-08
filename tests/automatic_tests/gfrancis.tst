getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/gfrancis_data.ref','rb');
Plant = ssrand(1, 3, 5);
[F,G,H,J] = abcd(Plant);
nw = 4;nuu = 2;A = rand(nw, nw);
st = maxi(real(spec(A)));A = A - st * eye(A);
B = rand(nw, nuu);C = 2 * rand(1, nw);D = 0 * rand(C * B);
Model = syslin('c', A, B, C, D);
[L,M,T] = gfrancis(Plant, Model);
%ans = norm(F * T + G * L - T * A, 1);
if load_ref('%ans') then   pause,end,

%ans = norm(H * T + J * L - C, 1);
if load_ref('%ans') then   pause,end,

%ans = norm(G * M - T * B, 1);
if load_ref('%ans') then   pause,end,

%ans = norm(J * M - D, 1);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
