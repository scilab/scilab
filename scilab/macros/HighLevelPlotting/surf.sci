function surf(varargin)

[lhs,rhs]=argn(0);

if rhs==0
   x=-1:0.1:1;
   y=x;
   deff('z=f(x,y)','z=cos(%pi*x.*y)');
   surf(x,y,f,'view',[45 30]);
else
   mainPlot3d('surf',varargin);
end

endfunction /////
