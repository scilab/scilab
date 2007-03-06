function h=mtlb_axes(varargin)
global AXES_T
if AXES_T==[] then AXES_T=list([]),end
win=xget('window')
k=find(AXES_T(1)==win)
if k==[] then
  AXES_T(1)=[AXES_T(1);win]
  AXES_T($+1)=[]
  k=size(AXES_T(1),1)+1
else
  k=k+1
end
if size(varargin)==0 then
  rect=[0 0 1 1]
  xsetech([0 0 1 1])
  AXES_T(k)=[AXES_T(k);rect];h=size(AXES_T(k),1)
elseif varargin(1)=='position'
  rect=matrix(varargin(2),1,-1)
  xsetech(rect)
  AXES_T(k)=[AXES_T(k);rect];h=size(AXES_T(k),1)
elseif type(varargin(1))==1 then
  xsetech(AXES_T(k)(varargin(1),:))
else
  error('mtlb_axes: This case is not yet implemented')
end
  
  
endfunction
