function mesh(varargin)

[lhs,rhs]=argn(0);

if rhs==0

   x=-1:0.1:1;
   y=x;
   deff('z=f(x,y)','z=cos(%pi*x.*y)');
   mesh(x,y,f,'view',[45 30]);

else

   state=loadGraphicState(gcw());
   cmap=state('colormap');

   varargin(0)=state('background');
   varargin(0)='facecolor';
   varargin(0)=cmap(1,:);
   varargin(0)='edgecolor';
   mainPlot3d('mesh',varargin);

end

endfunction /////
