getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/lqr_data.ref','rb');
A = rand(2, 2);B = rand(2, 1);//two states, one input
Q = diag([2,5]);R = 2;//Usual notations x'Qx + u'Ru
Big = sysdiag(Q, R);//Now we calculate C1 and D12
[w,wp] = fullrf(Big);C1 = wp(:, 1:2);D12 = wp(:, 3:$);//[C1,D12]'*[C1,D12]=Big
P = syslin('c', A, B, C1, D12);//The plant (continuous-time)
[K,X] = lqr(P);
if load_ref('X') then   pause,end,
if load_ref('K') then   pause,end,

%ans = spec(A + B * K);
if load_ref('%ans') then   pause,end,
//check stability
%ans = norm(A' * X + X * A - X * B * inv(R) * B' * X + Q, 1);
if load_ref('%ans') then   pause,end,
//Riccati check
P = syslin('d', A, B, C1, D12);// Discrete time plant
[K,X] = lqr(P);
if load_ref('X') then   pause,end,
if load_ref('K') then   pause,end,

%ans = spec(A + B * K);
if load_ref('%ans') then   pause,end,
//check stability
%ans = norm(A' * X * A - A' * X * B * pinv(B' * X * B + R) * (B' * X * A) + Q - X, 1);
if load_ref('%ans') then   pause,end,
//Riccati check
xdel_run(winsid());

mclose(%U);
