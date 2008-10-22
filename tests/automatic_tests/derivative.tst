getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/derivative_data.ref','rb');
function y=F(x)
   y=[sin(x(1)*x(2))+exp(x(2)*x(3)+x(1)) ; sum(x.^3)];
endfunction;
function y=G(x,p)
   y=[sin(x(1)*x(2)*p)+exp(x(2)*x(3)+x(1)) ; sum(x.^3)];
endfunction;

x = [1;2;3];[J,H] = derivative(F, x, H_form='blockmat');
if load_ref('H') then   pause,end,
if load_ref('J') then   pause,end,


n = 3;
// form an orthogonal matrix :
nu = 0;while nu < n then  [Q,nu] = colcomp(rand(n, n));end,
for i = [1,2,4],
  [J,H] = derivative(F, x, order=i, H_form='blockmat', Q=Q);
  mprintf('order= %d \n', i);
  H;
  if load_ref('H') then   pause,end,

end,

p = 1;h = 0.001;
[J,H] = derivative(list(G, p), x, h, 2, H_form='hypermat');H;
if load_ref('H') then   pause,end,

[J,H] = derivative(list(G, p), x, h, 4, Q=Q);H;
if load_ref('H') then   pause,end,


// Taylor series example:
dx = 0.001 * [1;1;-1];
[J,H] = derivative(F, x);
%ans = F(x + dx);
if load_ref('%ans') then   pause,end,

%ans = F(x + dx) - F(x);
if load_ref('%ans') then   pause,end,

%ans = F(x + dx) - F(x) - J * dx;
if load_ref('%ans') then   pause,end,

%ans = F(x + dx) - F(x) - J * dx - 1/2 * H * (dx .*. dx);
if load_ref('%ans') then   pause,end,


// A trivial example
function y=f(x,A,p,w)
, y=x'*A*x+p'*x+w; 
endfunction;
// with Jacobian and Hessean given by J(x)=x'*(A+A')+p', and H(x)=A+A'.
A = rand(3, 3);p = rand(3, 1);w = 1;
x = rand(3, 1);
[J,H] = derivative(list(f, A, p, w), x, h=1, H_form='blockmat');
if load_ref('H') then   pause,end,
if load_ref('J') then   pause,end,

// Since f(x) is quadratic in x, approximate derivatives of order=2 or 4 by finite
// differences should be exact for all h~=0. The apparent errors are caused by
// cancellation in the floating point operations, so a "big" h is choosen.
// Comparison with the exact matrices:
Je = x' * (A + A') + p';
if load_ref('Je') then   pause,end,

He = A + A';
if load_ref('He') then   pause,end,

%ans = clean(Je - J);
if load_ref('%ans') then   pause,end,

%ans = clean(He - H);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
