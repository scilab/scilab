function tripcolor(varargin)

[lhs,rhs]=argn(0);

if rhs==0
   clf()  
   colormap hot 
   load(PLOTLIB+'tridem.dat')
   tripcolor(nodes,xy(1,:),xy(2,:),P(:,1),'shading', ...
   'interp','view',[135 60],'colorbar','on', ...
   'background',[0 0 1],'axis','equal')
else
   mainPlot3d('tripcolor',varargin);
end

endfunction /////
