function [map,table]=activateRGBColormap(winNum,cmap)

[lhs,rhs]=argn(0);

if rhs==1
   Gstate=loadGlobalGraphicState();
   cmap=Gstate('defaultRGBcolormap');
end

state=loadGraphicState(winNum);
state('RGBcolormap')=cmap;
n1=size(state('colormap'),1);
n2=size(state('RGBcolormap'),1);  

xset('window',winNum);
xset('colormap',[state('colormap');state('RGBcolormap')]);
state('RGBcolormaptable')=n1+1:n1+n2;

xset('background',addcolor(state('frameColor')));
xset('foreground',addcolor(state('foreground')));

saveGraphicState(state,winNum);
