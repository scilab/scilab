function semilogx(varargin)

[lhs,rhs]=argn(0);

if ~rhs
   w=logspace(-2,2,200);
   s=%i*w;
   g=1../(s.^2+0.01*s+1);
   semilogx(w,abs(g));
   return
end

varargin(0) = 'tightX';
varargin(0) = 'axis';
varargin(0) = 'log';
varargin(0) = 'Xscale';
varargin(0) = 'linear';
varargin(0) = 'Yscale';

mainPlot('semilogx',varargin);

// end of semilogx
