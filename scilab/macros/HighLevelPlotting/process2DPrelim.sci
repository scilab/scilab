function [modeStart,modeAdd,modeScale,nTicksX,nTicksY]=process2DPrelim(win,liste,Xscale,Yscale,...
axisVect,axisTightX,axisTightY,axisRatio,axisStyle,colorBar,tab,matOfLegends,typeOfLegend,fontSize,winH)

nTicksX=[];
nTicksY=[];

state=loadGraphicState(win);

if state('nextPlot')=='erase'

	vp=state('viewport');

	if liste~=[]
	  state('listOfPlots')=liste;
	end

	// set the number of tickmarks, depending of current subplot state

	nTicksX = max(5,int(vp(3)*5));
	nTicksY = max(5,int(vp(4)*5));

	minX=axisVect(1); maxX=axisVect(2);
	minY=axisVect(3); maxY=axisVect(4);

	if ~ (axisTightX)  // else if the axis has not been told to be 'tight'
    	 [minX,maxX,nTicksX]=graduate(minX,maxX); // do the pretty axis graduations, thanks M. Steer
	end
	if ~ (axisTightY)
    	 [minY,maxY,nTicksY]=graduate(minY,maxY);
	end

	if ((maxY-minY)<%eps)
		maxY=maxY*(1+2*%eps)+1;
		minY=minY*(1-2*%eps)-1;
	end

	state('axis')=[minX minY maxX maxY];

	if axisRatio=='equal'
	  state('axisMode')='equal';
	end

	[vp,ech,cbech,xlech,ylech,titech]=changeVP(state,colorBar)

	state('viewport')=vp;
	state('ech')=ech;
	state('cbech')=cbech;
	state('xlech')=xlech;
	state('ylech')=ylech;
    state('titech')=titech;

else
	L=length(state('listOfPlots'));
    if liste~=[]
      for i=1:length(liste) // concatenate the new list of plots.
          state('listOfPlots')(L+i)=liste(i);
      end
    end
end

state('Xscale')=Xscale;
state('Yscale')=Yscale;

select axisStyle
   case 'off'
      z='0';
   case 'box'
      z='2';
   case 'normal'
      z='1',
   case 'right'
      z='3';
   case 'center'
      z='4';
   case 'origin'
      z='5'
end

y='1';

if state('axisMode')=='equal'
   y='3';
end



modeStart='0'+y+z;

modeAdd='0'+y+z;

modeScale='';

if state('nextPlot')=='erase' 
      modeAdd='00'+z;
end

if state('Xscale')=='log'
   modeScale=modeScale+'l';
else
   modeScale=modeScale+'n';
end

if state('Yscale')=='log'
   modeScale=modeScale+'l';
else
   modeScale=modeScale+'n';
end

setCurrentViewport(state,win,modeScale)
