function [zu,zv]=nonParametricDiffDeriv(theSurf,u,v,z)

h=1e-7;
[zu]=theSurf(u+h,v+%eps);
[zv]=theSurf(u+%eps,v+h);

zu=(zu(:)-z(:))/h;
zv=(zv(:)-z(:))/h;

endfunction
