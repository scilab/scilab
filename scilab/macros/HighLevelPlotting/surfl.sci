function surfl(varargin)

[lhs,rhs]=argn(0);

if rhs==0
   x=-2:0.1:2;
   y=x;
   deff('z=f(x,y)','z=cos(%pi*sqrt(x.^2+y.^2))');
   clf();
   colormap blue
   surfl(x,y,f,'facecolor','interp','light',[1 0 1],'view',[52 52],'axis','vis3d');
else
   mainPlot3d('surfl',varargin);
end

endfunction /////
