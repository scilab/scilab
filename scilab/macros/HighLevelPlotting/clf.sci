function clf(varargin)

global myWorld

if length(myWorld)==0
   graphinit()
end

if winsid()==[]
   fig();
end

globalState=loadGlobalGraphicState();

[lhs,rhs]=argn(0);

if rhs==1
   win=varargin(1);
else
   f=gcf();
   win=f.figure_id;
end

s=loadGraphicState(win);
s('nextPlot')='erase';
s('subplotState')='normal';
s('viewport')=globalState('defaultViewport');
s('Xscale')='linear';
s('Yscale')='linear'
s('listOfPlots')=list();

saveGraphicState(s,win);  

clearWindow(win)

// end of clf 
