function ylabel(labelString) 

state=loadGraphicState(gcw());
xset('pattern',xget('foreground'))
xtitle(" "," ",labelString);
