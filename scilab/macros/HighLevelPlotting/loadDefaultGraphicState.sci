function [state]=loadDefaultGraphicState()

// Create a default graphic state. This function is 
// called by fig(). 

global myWorld

if length(myWorld)==0
   graphinit();
end

global myWorld

globalGraphicState=myWorld('globalGraphicState');

   ggs=globalGraphicState('bg');

   clum = ([.298936021 .58704307445 .114020904255]*ggs' >= .5) + 1;
   if clum==1
         fg = [1 1 1];
         fbg = 0.7*ggs + .3*fg;
	 bg=ggs;
    else
         fg=[0 0 0];
	 fbg=0.7*ggs;
	 bg=ggs;
    end

   state=tlist(['graphicState';
'axisList';
'mainAxis';
'nextPlot';
'viewport';
'ech';
'cbech';
'xlech';
'ylech';
'titech';
'background';
'foreground';
'frameColor';
'subplotState';
'axis';
'axisMode';
'Xscale';
'Yscale';
'colormap';
'RGBcolormap';
'RGBcolormaptable';
'caxisMode';
'caxis';
'listOfPlots';
'margin';
'colorbar'],...
[],...
[],...
'erase',...
globalGraphicState('defaultViewport'),...
[],...
[],...
[],...
[],...
[],...
bg,...
fg,...
fbg,...
'normal',...
[0 0 1 1],...
'normal',...
'linear',...
'linear',...
 globalGraphicState('defaultColormap'),...
 globalGraphicState('defaultRGBcolormap'),...
 [],...
 'auto',...
 [],...
 list(),...
 1/10,...
 'off');
 
// cmap1=state('colormap'); // default colormap for 1D objects plots
// cmap2=state('RGBcolormap');

// n1=size(cmap1,1);
// n2=size(cmap2,1)
// state('RGBcolormaptable')=n1+1:n1+n2;

// end of loadDefaultGraphicState

