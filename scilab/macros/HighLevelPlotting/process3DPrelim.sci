function [modeStart]=process3DPrelim(win,axisVect,axisRatio,axisStyle,colorBar,tab,fontSize)
if colorBar==[]
colorBar='off'
end
state=loadGraphicState(win);

viewport = state('viewport');

if state('nextPlot')=='erase'

	if fontSize==-1
		setFontSize(viewport)
	else
		xset('font size',fontSize);
	end

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

	if colorBar~='off'
		processColorBar(tab,colorBar,state);
	end

	xsetech(wrect=state('ech'),arect=[0 0 0 0]);  

else
   modeStart=[0 0];
end




saveGraphicState(state,win); // save the state of the current window
