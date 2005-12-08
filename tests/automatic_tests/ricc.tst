getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/ricc_data.ref','rb');
//Standard formulas to compute Riccati solutions
A = rand(3, 3);B = rand(3, 2);C = rand(3, 3);C = C * C';R = rand(2, 2);R = R * R' + eye();
B = B * inv(R) * B';
X = ricc(A, B, C, 'cont');
%ans = norm(A' * X + X * A - X * B * X + C, 1);
if load_ref('%ans') then   pause,end,

H = [A,-B;-C,-A'];
[T,d] = schur(eye(H), H, 'cont');T = T(:, 1:d);
X1 = T(4:6, :)/T(1:3, :);
%ans = norm(X1 - X, 1);
if load_ref('%ans') then   pause,end,

[T,d] = schur(H, 'cont');T = T(:, 1:d);
X2 = T(4:6, :)/T(1:3, :);
%ans = norm(X2 - X, 1);
if load_ref('%ans') then   pause,end,

//       Discrete time case
F = A;B = rand(3, 2);G1 = B;G2 = R;G = G1/G2 * G1';H = C;
X = ricc(F, G, H, 'disc');
%ans = norm(F' * X * F - F' * X * G1/(G2 + G1' * X * G1) * (G1' * X * F) + H - X);
if load_ref('%ans') then   pause,end,

H1 = [eye(3, 3),G;zeros(3, 3),F'];
H2 = [F,zeros(3, 3);-H,eye(3, 3)];
[T,d] = schur(H2, H1, 'disc');T = T(:, 1:d);X1 = T(4:6, :)/T(1:3, :);
%ans = norm(X1 - X, 1);
if load_ref('%ans') then   pause,end,

Fi = inv(F);
Hami = [Fi,Fi * G;H * Fi,F' + H * Fi * G];
[T,d] = schur(Hami, 'd');T = T(:, 1:d);
Fit = inv(F');
Ham = [F + G * Fit * H,-G * Fit;-Fit * H,Fit];
[T,d] = schur(Ham, 'd');T = T(:, 1:d);X2 = T(4:6, :)/T(1:3, :);
%ans = norm(X2 - X, 1);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
