function loglog(varargin)

[lhs,rhs]=argn(0);

if ~rhs
   w=logspace(-2,2,200);
   s=%i*w;
   g=1../(s.^2+0.01*s+1);
   loglog(w,abs(g));
   return
end

varargin(0) = 'tight';
varargin(0) = 'axis';
varargin(0) = 'log';
varargin(0) = 'Xscale';
varargin(0) = 'log';
varargin(0) = 'Yscale';

mainPlot('loglog',varargin);

// end of loglog
