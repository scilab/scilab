function saveGraphicState(state,winNum) 

//
// Saves the graphic state for window winNum
//

global myWorld

graphicState=myWorld('graphicState');
graphicState(winNum+1)=state;
myWorld('graphicState')=graphicState;

// end of saveGraphicState

