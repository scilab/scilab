function xlabel(labelString) 


win=gcw()
state=loadGraphicState(win);
ma=state.mainAxis;

ech=state('xlech');

xsetech(wrect=ech,frect=[0 0 1 1],arect=[0 0 0 0]);
a=gca();
a.font_size=ma.font_size;
a.font_style=ma.font_style;
a.background=ma.parent.background;

rect=xstringl(0,0,labelString);

xstringb(0,0,labelString,1,1);
h=gce();
h.foreground=ma.font_color;

//setCurrentViewport(state,win)
