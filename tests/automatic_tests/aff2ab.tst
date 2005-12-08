getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/aff2ab_data.ref','rb');
// Lyapunov equation solver (one unknown variable, one constraint)
%ans = deff('Y=lyapunov(X,D)', '[A,Q]=D(:);Xm=X(:); Y=list(A''*Xm+Xm*A-Q)');
if load_ref('%ans') then   pause,end,

A = rand(3, 3);Q = rand(3, 3);Q = Q + Q';D = list(A, Q);dimX = [3,3];
[Aly,bly] = aff2ab(lyapunov, dimX, D);
[Xl,kerA] = linsolve(Aly, bly);Xv = vec2list(Xl, dimX);%ans = lyapunov(Xv, D);
if load_ref('%ans') then   pause,end,

Xm = Xv(:);%ans = A' * Xm + Xm * A - Q;
if load_ref('%ans') then   pause,end,


// Lyapunov equation solver with redundant constraint X=X'
// (one variable, two constraints) D is global variable
%ans = deff('Y=ly2(X,D)', '[A,Q]=D(:);Xm=X(:); Y=list(A''*Xm+Xm*A-Q,Xm''-Xm)');
if load_ref('%ans') then   pause,end,

A = rand(3, 3);Q = rand(3, 3);Q = Q + Q';D = list(A, Q);dimX = [3,3];
[Aly,bly] = aff2ab(ly2, dimX, D);
[Xl,kerA] = linsolve(Aly, bly);Xv = vec2list(Xl, dimX);%ans = ly2(Xv, D);
if load_ref('%ans') then   pause,end,


// Francis equations
// Find matrices X1 and X2 such that:
// A1*X1 - X1*A2 + B*X2 -A3 = 0
// D1*X1 -D2 = 0
%ans = deff('Y=bruce(X,D)', '[A1,A2,A3,B,D1,D2]=D(:),[X1,X2]=X(:);Y=list(A1*X1-X1*A2+B*X2-A3,D1*X1-D2)');
if load_ref('%ans') then   pause,end,

A1 = [-4,10;-1,2];A3 = [1;2];B = [0;1];A2 = 1;D1 = [0,1];D2 = 1;
D = list(A1, A2, A3, B, D1, D2);
[n1,m1] = size(A1);[n2,m2] = size(A2);[n3,m3] = size(B);
dimX = [m1,n2;m3,m2];
[Af,bf] = aff2ab(bruce, dimX, D);
[Xf,KerAf] = linsolve(Af, bf);Xsol = vec2list(Xf, dimX);
if load_ref('Xsol') then   pause,end,

%ans = bruce(Xsol, D);
if load_ref('%ans') then   pause,end,


// Find all X which commute with A
%ans = deff('y=f(X,D)', 'y=list(D(:)*X(:)-X(:)*D(:))');
if load_ref('%ans') then   pause,end,

A = rand(3, 3);dimX = [3,3];[Af,bf] = aff2ab(f, dimX, list(A));
[Xf,KerAf] = linsolve(Af, bf);[p,q] = size(KerAf);
Xsol = vec2list(Xf + KerAf * rand(q, 1), dimX);
C = Xsol(:);%ans = A * C - C * A;
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
