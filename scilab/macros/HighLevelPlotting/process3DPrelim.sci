function [modeStart]=process3DPrelim(win,axisVect,axisRatio,axisStyle,colorBar,tab,fontSize)

if colorBar==[]
colorBar='off'
end

state=loadGraphicState(win);

viewport = state('viewport');

if state('nextPlot')=='erase'

	minX=axisVect(1); maxX=axisVect(2);
	minY=axisVect(3); maxY=axisVect(4);
	minZ=axisVect(5); maxZ=axisVect(6);

	state('axis')=[minX minY minZ maxX maxY maxZ];

	// default values

	typ=1;
	box=4;

	if axisStyle=='off'  
	  box=0;
	elseif axisStyle=='box'
	  box=3;
	elseif axisStyle=='trihedral'
	  box=2;
	end

	if axisRatio=='vis3d'
	  typ=3;
	elseif axisRatio=='equal'
	  typ=5;
	end

	modeStart=[typ box];

	[vp,ech,cbech,xlech,ylech,titech]=changeVP(state,colorBar)

	state('viewport')=vp;
	state('ech')=ech;
	state('cbech')=cbech;

else
   modeStart=[0 0];
end


setCurrentViewport(state,win,[])
