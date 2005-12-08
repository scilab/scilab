getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/binomial_data.ref','rb');
// first example
n = 10;p = 0.3;xbasc_run();plot2d3(0:n, binomial(p, n));

// second example
n = 50;p = 0.4;
mea = n * p;sigma = sqrt(n * p * (1 - p));
x = ((0:n) - mea)/sigma;
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

plot2d(x, sigma * binomial(p, n));
%ans = deff('y=Gauss(x)', 'y=1/sqrt(2*%pi)*exp(-(x.^2)/2)');
if load_ref('%ans') then   pause,end,

plot2d(x, Gauss(x), style=2);

// by binomial formula (Caution if big n)
function pr=binomial2(p,n)
  x=poly(0,'x');pr=coeff((1-p+x)^n).*horner(x^(0:n),p);
endfunction
if load_ref('%ans') then   pause,end,

p = 1/3;n = 5;
%ans = binomial(p, n) - binomial2(p, n);
if load_ref('%ans') then   pause,end,


// by Gamma function: gamma(n+1)=n! (Caution if big n)
p = 1/3;n = 5;
Cnks = gamma(n + 1) ./ (gamma(1:n + 1) .* gamma(n + 1:-1:1));
x = poly(0, 'x');
pr = Cnks .* horner(x.^(0:n) .* ((1 - x)^(n:-1:0)), p);
%ans = pr - binomial(p, n);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
