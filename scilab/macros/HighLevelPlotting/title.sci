function title(labelString) 

win=gcw()
state=loadGraphicState(win);
xset('pattern',xget('foreground'))
ech=state('titech');

xsetech(wrect=ech,frect=[0 0 1 1],arect=[0 0 0 0]);

rect=xstringl(0,0,labelString);

xstring(.5-rect(3)/2,.5-rect(4)/2,labelString);

//setCurrentViewport(state,win)
