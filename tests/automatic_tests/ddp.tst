getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/ddp_data.ref','rb');
rand('seed', 0);nx = 6;nz = 3;nu = 2;ny = 1;
A = diag(1:6);A(2, 2) = -7;A(5, 5) = -9;B2 = [1,2;0,3;0,4;0,5;0,0;0,0];
C1 = [zeros(nz, nz),eye(nz, nz)];D12 = [0,1;0,2;0,3];
Sys12 = syslin('c', A, B2, C1, D12);
C = [C1;rand(ny, nx)];D2 = [D12;rand(ny, size(D12, 2))];
Sys = syslin('c', A, B2, C, D2);
[A,B2,C1,D12] = abcd(Sys12);//The matrices of Sys12.
alfa = -1;beta = -2;flag = 'ge';
[X,dims,F,U,k,Z] = abinv(Sys12, alfa, beta, flag);
%ans = clean(X' * (A + B2 * F) * X);
if load_ref_nocheck('%ans') then   pause,end,

%ans = clean(X' * B2 * U);
if load_ref_nocheck('%ans') then   pause,end,

%ans = clean((C1 + D12 * F) * X);
if load_ref_nocheck('%ans') then   pause,end,

clean(D12 * U);
//Calculating an ad-hoc B1,D1
G1 = rand(size(B2, 2), 3);
B1 = -B2 * G1;
D11 = -D12 * G1;
D1 = [D11;rand(ny, size(B1, 2))];

[Closed,F,G] = ddp(Sys, 1:nz, B1, D1, 'st', alfa, beta);
closed = syslin('c', A + B2 * F, B1 + B2 * G, C1 + D12 * F, D11 + D12 * G);
%ans = ss2tf(closed);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
