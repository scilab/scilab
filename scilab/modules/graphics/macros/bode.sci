// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function []=bode(varargin)
  rhs=size(varargin)
  if type(varargin($))==10 then
    comments=varargin($),rhs=rhs-1;
  else
    comments=[];
  end
  fmax=[]
  if or(typeof(varargin(1))==['state-space' 'rational']) then 
    //sys,fmin,fmax [,pas] or sys,frq
    if rhs==1 then
      [frq,repf]=repfreq(varargin(1),1d-3,1d3)
    elseif rhs==2 then //sys,frq
      if size(varargin(2),2)<2 then
	error(msprintf(_("%s : Invalid argument #%d. It must be a row vector with length > %d"),..
		     "bode",1,1))
      end
      [frq,repf]=repfreq(varargin(1:rhs))
    elseif or(rhs==(3:4)) then //sys,fmin,fmax [,pas]
      [frq,repf]=repfreq(varargin(1:rhs))
    else
      error(msprintf(_("%s : Invalid call: sys,fmin,fmax [,pas] [,com]'),"bode"))
    end
    [phi,d]=phasemag(repf)
    if rhs>=3 then fmax=varargin(3),end
  elseif  type(varargin(1))==1 then 
    //frq,db,phi [,comments] or frq, repf [,comments]
    select rhs
    case 2 then //frq,repf
      frq=varargin(1);
      if size(frq,2)<2 then
	error(msprintf(_("%s : Invalid argument #%d. It must be a row vector with length > %d"),..
		     "bode",1,1))
      end
      if size(frq,2)<>size(varargin(2),2) then
	error(msprintf(_("%s : Incompatible dimensions of arguments #%d and #%d."),..
			 "bode",1,2))
      end
      [phi,d]=phasemag(varargin(2))
    case 3 then  //frq,db,phi
      [frq,d,phi]=varargin(1:rhs)
      if size(frq,2)<>size(d,2) then
	error(msprintf(_("%s : Incompatible dimensions of arguments #%d and #%d."),..
			 "bode",1,2))
      end
      if size(frq,2)<>size(phi,2) then
	error(msprintf(_("%s : Incompatible dimensions of arguments #%d and #%d."),..
			 "bode",1,3))
      end
    else 
      error(msprintf(_("%s : Invalid call: frq, db,phi [,com] or frq,repf [,com]'),"bode"))
    end
  else
    error(msprintf(_("%s : Invalid argument #%d. It must be a linear"+..
		     " dynamical system (syslin) or a real array"),"bode",1))
  end;
   frq=frq';d=d',phi=phi'
  [n,mn]=size(d)

  if comments==[] then
    hx=0.48
  else
    if size(comments,'*')<>mn then
      error(msprintf(_("%s : Invalid dimension for argument #%d"),"bode",rhs+1))
    end
    hx=0.43
  end;

  drawlater()
  sciCurAxes=gca();
  axes=sciCurAxes;
  wrect=axes.axes_bounds;


  //magnitude
  axes.axes_bounds=[wrect(1)+0,wrect(2)+0,wrect(3)*1.0,wrect(4)*hx*0.95]
  axes.data_bounds = [min(frq),min(d);max(frq),max(d)];
  axes.log_flags = "lnn" ;
  axes.grid=color('lightgrey')*ones(1,3);
  axes.axes_visible="on";
  if size(d,2)>1&size(frq,2)==1 then
    xpolys(frq(:,ones(1,mn)),d,1:mn)
  else
    xpolys(frq,d,1:mn)
  end
  if fmax<>[]&max(frq)<fmax then
      xpoly(max(frq)*[1;1],axes.y_ticks.locations([1 $]));e=gce();
      e.foreground=5;
  end
  xtitle('Magnitude ',' Hz','db');

  //phase
  axes=newaxes();
  axes.axes_bounds=[wrect(1)+0,wrect(2)+wrect(4)*hx,wrect(3)*1.0,wrect(4)*hx*0.95];
  axes.data_bounds = [mini(frq),mini(phi);maxi(frq),maxi(phi)];
  axes.log_flags = "lnn" ;
  axes.grid=color('lightgrey')*ones(1,3);
  axes.axes_visible="on";
  if size(phi,2)>1&size(frq,2)==1 then
    xpolys(frq(:,ones(1,mn)),phi,1:mn)
  else
    xpolys(frq,phi,1:mn)
  end
  if fmax<>[]&max(frq)<fmax then
      xpoly(max(frq)*[1;1],axes.y_ticks.locations([1 $]));e=gce();
      e.foreground=5;
  end
  // create legend
  if comments<>[] then
    axes=newaxes()
    axes.axes_bounds=[wrect(1)+0,wrect(2)+wrect(4)*2*hx,wrect(3)*1.0,wrect(4)*0.1];
    axes.data_bounds=[0 0; 1 1];
    y0=0.7;dy=-1/2
    x0=0;dx=1/2
    count=0
    for k=1:mn
      xsegs([x0;x0+0.08],[y0;y0],k),e1=gce()
      rect=xstringl(x0+0.1,y0,comments(k))
      xstring(x0+0.1,y0-rect(4)/3,comments(k)),e2=gce()
      glue([e2 e1])
      count=count+1
      y0=y0+dy
      if count==3 then x0=x0+dx;y0=0.7,end
    end
  end
  drawnow()
  // return to the previous scale
  set( "current_axes", sciCurAxes ) ;

endfunction
