function pcolor(varargin)

[lhs,rhs]=argn(0);

if rhs==0
   r=linspace(0,1,25);
   theta=%pi/4+linspace(0,3*%pi/2,25);
   [R,T]=meshgrid(r,theta);
  
pcolor(R.*cos(T),R.*sin(T),rand(T,'normal'),'shading','interp','colorbar','on','axis','equal')
else
   mainPlot3d('pcolor',varargin);
end

endfunction /////
