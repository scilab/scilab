getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/obscont_data.ref','rb');
ny = 2;nu = 3;nx = 4;P = ssrand(ny, nu, nx);[A,B,C,D] = abcd(P);
Kc = -ppol(A, B, [-1,-1,-1,-1]);//Controller gain
Kf = -ppol(A', C', [-2,-2,-2,-2]);Kf = Kf';//Observer gain
cl = P /. (-obscont(P, Kc, Kf));%ans = spec(cl('A'));
if load_ref('%ans') then   pause,end,
//closed loop system
[J,r] = obscont(P, Kc, Kf);
Q = ssrand(nu, ny, 3);Q('A') = Q('A') - (maxi(real(spec(Q('A')))) + 0.5) * eye(Q('A'));
if load_ref('Q') then   pause,end,

//Q is a stable parameter
K = lft(J, r, Q);
%ans = spec(h_cl(P, K));
if load_ref('%ans') then   pause,end,
// closed-loop A matrix (should be stable);
xdel_run(winsid());

mclose(%U);
