function varargout=mtlb_axis(varargin)
// Copyright INRIA
// Emulation function for Matlab axis()
// V.C.

varargout(1)=list()
listvar=varargin
if size(listvar)==0
  a=gca();
  set(gca(),"axes_visible",'on')
  varargout(1)=matrix(a.data_bounds,1,-1);
  return
elseif type(listvar(1))<>9 then
  a=gca();
  rhs=lstsize(listvar)
else
  a=listvar(1)
  listvar(1)=null()
  rhs=lstsize(listvar)
  listvar=listvar
end

if rhs>0 then
  for krhs=1:rhs
    if type(listvar(krhs))==1 then
        if size(listvar(krhs),1)>1 & size(listvar(krhs),2)>1 then
	  error("a.data_bounds must be a vector")
	end
        listvar(krhs) = matrix(listvar(krhs),1,-1);
	
      // axis([xmin xmax ymin ymax zmin zmax])
      if size(listvar(krhs),2)==4 then
        set(gca(),"data_bounds",matrix(listvar(krhs),2,-1))
	set(gca(),"view",'2d')
	set(gca(),"axes_visible",'on')
      elseif size(listvar(krhs),2)==6
	set(gca(),"data_bounds",matrix(listvar(krhs),2,-1))
	set(gca(),"view",'3d')
	set(gca(),"axes_visible",'on')
      // axis([xmin xmax ymin ymax zmin zmax cmin cmax]) 
      elseif size(listvar(krhs),2)==8 then
	error("a.data_bounds=[xmin xmax ymin ymax zmin zmax cmin cmax] not implemented!");
      // Unknown column number for listvar(krhs)
      else
	error("Bad affectation for a.data_bounds!");
      end
      
    elseif type(listvar(krhs))==10 then
	
      // axis auto
      if listvar(krhs)=="auto" then
	a.auto_scale="on"
	
	// axis manual
      elseif listvar(krhs)=="manual" then
	a.auto_scale="off"
	
	// axis tight
      elseif listvar(krhs)=="tight" then
	a.tight_limits="on"
	
	// axis fill
      elseif listvar(krhs)=="fill" then
	error("axis fill not implemented!");

	// axis ij
      elseif listvar(krhs)=="ij" then
	a.rotation_angles=[180 270]
	
	// axis xy
      elseif listvar(krhs)=="xy" then
	a.rotation_angles=[0 270]
	
	// axis equal
      elseif listvar(krhs)=="equal" then
	a.isoview="on"
	
	// axis image
      elseif listvar(krhs)=="image" then
	error("axis image not implemented!");

	// axis square
      elseif listvar(krhs)=="square" then
	if a.view=="2d" then
	  warning("cube_scaling only used in 3d mode");
	end
	a.cube_scaling="on"
	
	// axis vis3d
      elseif listvar(krhs)=="vis3d" then
	a.view="3d"
	
	// axis normal
      elseif listvar(krhs)=="normal" then
	error("axis normal not implemented!");
	
	// axis on
      elseif listvar(krhs)=="on" then
	a.axes_visible="on"
	
	// axis off
      elseif listvar(krhs)=="off" then
	a.axes_visible="off"
	
	// [mode,visibility,direction] = axis('state')
      elseif listvar(krhs)=="state" then
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
	error("axis "+listvar(krhs)+" not implemented!");
      end
      
    // axis(axes_handles,...)
    elseif type(listvar(krhs))==9 then
      // krhs must be one
      for khandle=1:lstsize(listvar(krhs))
	arglist=list()
	for kvararg=1:lstsize(listvar)-1
	  arglist($+1)=listvar(kvararg+1)
	end
	arglist($+1)=listvar(krhs)(khandle)
	mtlb_axis(arglist)
      end
    // Wrong type for listvar(krhs)
    else
	error("Argument of type "+string(type(listvar(krhs)))+" not implemented!");
    end
    varargout(1)=matrix(a.data_bounds,1,-1);
  end

// v = axis
else
  varargout(1)=matrix(a.data_bounds,1,-1);
end
endfunction
