function [state]=loadGraphicState(winNum) 

global myWorld

if length(myWorld)==0
   graphinit();
end

global myWorld

graphicState=myWorld('graphicState');
globalGraphicState=myWorld('globalGraphicState');

[state,err]=evstr('graphicState(winNum+1)');

// If the window did not exist before, we create for it
// a default graphic state :

if length(state)==0
   state=loadDefaultGraphicState();
end

// end of loadGraphicState

