function [foreground,background]=processSFB(foreground_in,background_in,win,typeOfPlot)

state=loadGraphicState(win);

globalState=loadGlobalGraphicState();

background=background_in;
foreground=foreground_in;

if background==[]
   background=state('background');
end

if foreground==[]
   foreground=state('foreground');
end

// 

// Now prevent the xbasc() if there has been a first subplot() or a `hold on'

if state('nextPlot')=='erase'
	state('axisMode')='normal';
	state('axis')=[];
	if state('subplotState')=='normal'
		state('viewport')=globalState('defaultViewport');
	end
	saveGraphicState(state,win);
	if state('subplotState')=='first' | state('subplotState')=='normal'
		clearWindow(win,typeOfPlot);
	end
	
end
