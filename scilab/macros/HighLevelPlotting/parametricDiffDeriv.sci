function [xu,yu,zu,xv,yv,zv]=parametricDiffDeriv(theSurf,u,v,x,y,z)

h=1e-7;
[xu,yu,zu]=theSurf(u+h,v+%eps);
[xv,yv,zv]=theSurf(u+%eps,v+h);

xu=(xu-x)/h; yu=(yu-y)/h; zu=(zu-z)/h;
xv=(xv-x)/h; yv=(yv-y)/h; zv=(zv-z)/h;

endfunction
