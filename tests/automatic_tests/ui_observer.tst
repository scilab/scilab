getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/ui_observer_data.ref','rb');
A = diag([3,-3,7,4,-4,8]);
B = [eye(3, 3);zeros(3, 3)];
C = [0,0,1,2,3,4;0,0,0,0,0,1];
D = [1,2,3;0,0,0];
rand('seed', 0);w = ss2ss(syslin('c', A, B, C, D), rand(6, 6));
[A,B,C,D] = abcd(w);
B = [B,matrix(1:18, 6, 3)];D = [D,matrix(-(1:6), 2, 3)];
reject = 1:3;
Sys = syslin('c', A, B, C, D);
N1 = [-2,-3];C1 = -N1 * C;D1 = -N1 * D;
nw = length(reject);nu = size(Sys('B'), 2) - nw;
ny = size(Sys('C'), 1);nz = size(C1, 1);
[UIobs,J,N] = ui_observer(Sys, reject, C1, D1);

W = [[zeros(nu, nw),eye(nu, nu)];Sys];UIobsW = UIobs * W;
//(w,u) --> z=UIobs*[0,I;Sys](w,u)
clean(ss2tf(UIobsW));
wu_to_z = syslin('c', A, B, C1, D1);clean(ss2tf(wu_to_z));
%ans = clean(ss2tf(wu_to_z) - ss2tf(UIobsW), 0.0000001);
if load_ref('%ans') then   pause,end,

/////2nd example//////
nx = 2;ny = 3;nwu = 2;Sys = ssrand(ny, nwu, nx);
C1 = rand(1, nx);D1 = [0,1];
UIobs = ui_observer(Sys, 1, C1, D1);
xdel_run(winsid());

mclose(%U);
