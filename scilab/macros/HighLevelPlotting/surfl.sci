function surfl(varargin)

[lhs,rhs]=argn(0);

if rhs==0
   x=-1:0.1:1;
   y=x;
   deff('z=f(x,y)','z=cos(%pi*x.*y)');
   surfl(x,y,f,'shading','flat','light',[1 0 0],'view',[45 30]);
else
   mainPlot3d('surfl',varargin);
end

endfunction /////
