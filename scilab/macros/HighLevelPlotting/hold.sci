function hold(varargin) 

[lhs,rhs]=argn(0);

if winsid()==[]
   return
end

winNum=xget('window');
state=loadGraphicState(winNum);

if rhs==1
  if varargin(1)=='on'
    state('nextPlot')='add'
  elseif varargin(1)=='off'
    state('nextPlot')='erase'
  else
    error('hold : unknown hold state (must be ''on'' or ''off'')')
  end
elseif rhs==0
  if state('nextPlot')=='add'
    state('nextPlot')='erase'
    disp('Current plot released')
  else
    state('nextPlot')='add'
    disp('Current plot held')
  end
end

saveGraphicState(state,winNum);
