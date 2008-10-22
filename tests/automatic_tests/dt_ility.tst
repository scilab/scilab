getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/dt_ility_data.ref','rb');
A = [2,1,1;0,-2,1;0,0,3];
C = [0,0,1];
X = rand(3, 3);A = inv(X) * A * X;C = C * X;
W = syslin('c', A, [], C);
[k,n,U,W1] = dt_ility(W);
%ans = W1('A');
if load_ref('%ans') then   pause,end,

%ans = W1('C');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
