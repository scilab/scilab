function c=colormap(varargin)

c=[];

[lhs,rhs]=argn(0);

if rhs==0
   state=loadGraphicState(gcw());
   c=(state('RGBcolormap'))
elseif rhs==1
   fig(gcw(),'colormap',varargin(1));
else
   error('colormap : two many input arguments');
end
