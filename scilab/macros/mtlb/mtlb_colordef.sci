function varargout=mtlb_colordef(varargin)
// Copyright INRIA
// Emulation function for Matlab colordef()
// V.C.

lhs=argn(1);

if lstsize(varargin)==1 then
  color_option=varargin(1)
  fig=[]
  
  f=gcf()
  if or(color_option==["black","none"]) then
    f.background=-1
  else
    f.background=-2
  end
  if lhs==1 then
    varargout(1)=f
  end
else
  color_option=varargin(2)
  fig=varargin(1)
  
  if type(fig)==10 then //'new'
    fig=scf(max(winsid())+1);
  end
  
  if or(color_option==["black","none"]) then
    fig.background=-1
  else
    fig.background=-2
  end
  if lhs==1 then
    varargout(1)=fig
  end
  
end


endfunction


