function pr=binomial(p,n)
//binomial probability distribution
//pr(k) = probability (X=k) , with X=B(n,p)
//Example:
//n=10;p=0.3;plot2d3("onn",(0:n)',binom(p,n)');
//n=100;p=0.3;
//moy=n*p;sigma=sqrt(n*p*(1-p));
//x=( (0:n)-moy )/sigma;
//plot2d(x,sigma*binom(p,n));
//deff('y=gauss(x)','y=1/sqrt(2*%pi)*exp(-(x.^2)/2)')
//plot2d(x,gauss(x));
z=poly(0,'z');
pr=coeff((1-p+z)^n).*horner(z^(0:n),p);
