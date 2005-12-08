getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/spec_data.ref','rb');
// MATRIX EIGENVALUES
A = diag([1,2,3]);X = rand(3, 3);A = inv(X) * A * X;
%ans = spec(A);
if load_ref('%ans') then   pause,end,

//
x = poly(0, 'x');
pol = det(x * eye() - A);
if load_ref('pol') then   pause,end,

%ans = roots(pol);
if load_ref('%ans') then   pause,end,

//
[S,X] = bdiag(A);
%ans = clean(inv(X) * A * X);
if load_ref('%ans') then   pause,end,


// PENCIL EIGENVALUES
A = rand(3, 3);
[al,be,Z] = spec(A, eye(A));%ans = al ./ be;
if load_ref('%ans') then   pause,end,

%ans = clean(inv(Z) * A * Z);
if load_ref('%ans') then   pause,end,
//displaying the eigenvalues (generic matrix)
A = A + %i * rand(A);E = rand(A);
%ans = roots(det(%s * E - A));
if load_ref('%ans') then   pause,end,
//complex case


xdel_run(winsid());

mclose(%U);
