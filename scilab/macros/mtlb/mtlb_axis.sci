function [varargout]=mtlb_axis(varargin)
// Copyright INRIA
// Emulation function for Matlab axis()
// V.C.

if type(varargin($))<>9 then
  a=gca();
  rhs=lstsize(varargin)
else
  a=varargin($)
  rhs=lstsize(varargin)-1
end

if rhs>0 then
  for krhs=1:rhs
    if type(varargin(krhs))==1 then
      
      // axis([xmin xmax ymin ymax])
      // axis([xmin xmax ymin ymax zmin zmax])
      if or(size(varargin(krhs),2)==[4,6]) then
	a.data_bounds=matrix(varargin(krhs),2,-1);
	
      // axis([xmin xmax ymin ymax zmin zmax cmin cmax]) 
      elseif size(varargin(krhs),2)==8 then
	error("a.data_bounds=[xmin xmax ymin ymax zmin zmax cmin cmax] not implemented!");
      // Unknown column number for varargin(krhs)
      else
	error("Bad affectation for a.data_bounds!");
      end
      
    elseif type(varargin(krhs))==10 then
	
      // axis auto
      if varargin(krhs)=="auto" then
	a.auto_scale="on"
	
	// axis manual
      elseif varargin(krhs)=="manual" then
	a.auto_scale="off"
	
	// axis tight
      elseif varargin(krhs)=="tight" then
	a.tight_limits="on"
	
	// axis fill
      elseif varargin(krhs)=="fill" then
	error("axis fill not implemented!");

	// axis ij
      elseif varargin(krhs)=="ij" then
	a.rotation_angles=[180 270]
	
	// axis xy
      elseif varargin(krhs)=="xy" then
	a.rotation_angles=[0 270]
	
	// axis equal
      elseif varargin(krhs)=="equal" then
	a.isoview="on"
	
	// axis image
      elseif varargin(krhs)=="image" then
	error("axis image not implemented!");

	// axis square
      elseif varargin(krhs)=="square" then
	if a.view=="2d" then
	  warning("cube_scaling only used in 3d mode");
	end
	a.cube_scaling="on"
	
	// axis vis3d
      elseif varargin(krhs)=="vis3d" then
	a.view="3d"
	
	// axis normal
      elseif varargin(krhs)=="normal" then
	error("axis normal not implemented!");
	
	// axis on
      elseif varargin(krhs)=="on" then
	a.axes_visible="on"
	
	// axis off
      elseif varargin(krhs)=="off" then
	a.axes_visible="off"
	
	// [mode,visibility,direction] = axis('state')
      elseif varargin(krhs)=="state" then
	if a.auto_scale=="on" then
	  varargout(1)="auto"
	else
	  varargout(1)="manual"
	end
	varargout(2)=a.axes_visible
	if a.rotation_angles==[0 180] then
	  varargout(3)="xy"
	else
	  varargout(3)="ij"
	end
	// Unknown character string
      else
	error("axis "+varargin(krhs)+" not implemented!");
      end
      
    // axis(axes_handles,...)
    elseif type(varargin(krhs))==9 then
      // krhs must be one
      for khandle=1:lstsize(varargin(krhs))
	arglist=list()
	for kvararg=1:lstsize(varargin)-1
	  arglist($+1)=varargin(kvararg+1)
	end
	arglist($+1)=varargin(krhs)(khandle)
	mtlb_axis(arglist)
      end
    // Wrong type for varargin(krhs)
    else
	error("Argument of type "+string(type(varargin(krhs)))+" not implemented!");
    end
  end

// v = axis
else
  varargout(1)=a.data_bounds;
end
endfunction
