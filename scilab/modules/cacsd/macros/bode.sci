// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  1985-2010 - INRIA - Serge Steer
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
  fname="bode";//for error messages
  fmax=[]
  discr=%f //for shannon limit
  if or(typeof(varargin(1))==["state-space" "rational"]) then
    //sys,fmin,fmax [,pas] or sys,frq
    refdim=1 //for error message
    discr=varargin(1).dt<>'c';
    if rhs==1 then //sys
      [frq,repf]=repfreq(varargin(1),1d-3,1d3)
    elseif rhs==2 then //sys,frq
      if size(varargin(2),2)<2 then
        error(msprintf(_("%s: Wrong size for input argument #%d: A row vector with length>%d expected.\n"),..
                       fname,2,1))
      end
      [frq,repf]=repfreq(varargin(1:rhs))
    elseif or(rhs==(3:4)) then //sys,fmin,fmax [,pas]
      [frq,repf]=repfreq(varargin(1:rhs))
    else
      error(msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"),fname,1,5))
    end
    [phi,d]=phasemag(repf)
    if rhs>=3 then fmax=varargin(3),end
  elseif  type(varargin(1))==1 then
    //frq,db,phi [,comments] or frq, repf [,comments]
    refdim=2
    select rhs
    case 2 then //frq,repf
      frq=varargin(1);
      if size(frq,2)<2 then
        error(msprintf(_("%s: Wrong size for input argument #%d: A row vector with length>%d expected.\n"),..
                       fname,1,1))
      end
      if size(frq,2)<>size(varargin(2),2) then
        error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same column dimensions expected.\n"),..
                       fname,1,2))
      end
      [phi,d]=phasemag(varargin(2))
    case 3 then  //frq,db,phi
      [frq,d,phi]=varargin(1:rhs)
      if size(frq,2)<>size(d,2) then
        error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same column dimensions expected.\n"),..
                       fname,1,2))
      end
      if size(frq,2)<>size(phi,2) then
        error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same column dimensions expected.\n"),..
                       fname,1,3))
      end
    else
      error(msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"),fname,2,4))
    end
  else
    error(msprintf(_("%s: Wrong type for input argument #%d: Linear dynamical system or row vector of floats expected.\n"),fname,1))
  end;
  frq=frq';d=d',phi=phi'
  [n,mn]=size(d)

  if comments==[] then
    hx=0.48
  else
    if size(comments,"*")<>mn then
      error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same number of elements expected.\n"),...
                     fname,refdim,rhs+1))
    end
    hx=0.43
  end;

  fig=gcf();
  immediate_drawing=fig.immediate_drawing;
  fig.immediate_drawing="off";

  sciCurAxes=gca();
  axes=sciCurAxes;
  wrect=axes.axes_bounds;


  //magnitude
  axes.axes_bounds=[wrect(1)+0,wrect(2)+0,wrect(3)*1.0,wrect(4)*hx*0.95]
  axes.data_bounds = [min(frq),min(d);max(frq),max(d)];
  axes.log_flags = "lnn" ;
  axes.grid=color("lightgrey")*ones(1,3);
  axes.axes_visible="on";
  axes.clip_state = "clipgrf";
  if size(d,2)>1&size(frq,2)==1 then
    xpolys(frq(:,ones(1,mn)),d,1:mn)
  else
    xpolys(frq,d,1:mn)
  end
  //set datatips info
  e=gce();

  for i=1:size(e.children,"*")
    datatipInitStruct(e.children(i),"formatfunction","formatBodeMagTip")
  end

  if discr&fmax<>[]&max(frq)<fmax then
    xpoly(max(frq)*[1;1],axes.y_ticks.locations([1 $]));e=gce();
    e.foreground=5;
  end
  xtitle("",_("Frequency (Hz)"),_("Magnitude (dB)"));

  //phase

  axes=newaxes();
  axes.axes_bounds=[wrect(1)+0,wrect(2)+wrect(4)*hx,wrect(3)*1.0,wrect(4)*hx*0.95];
  axes.data_bounds = [min(frq),min(phi);max(frq),max(phi)];
  axes.log_flags = "lnn" ;
  axes.grid=color("lightgrey")*ones(1,3);
  axes.axes_visible="on";
  axes.clip_state = "clipgrf";
  if size(phi,2)>1&size(frq,2)==1 then
    xpolys(frq(:,ones(1,mn)),phi,1:mn)
  else
    xpolys(frq,phi,1:mn)
  end
  ephi=gce()
  //set datatips info
  for i=1:size(ephi.children,"*")
    datatipInitStruct(ephi.children(i),"formatfunction","formatBodePhaseTip")
  end

  if discr&fmax<>[]&max(frq)<fmax then
    xpoly(max(frq)*[1;1],axes.y_ticks.locations([1 $]));e=gce();
    e.foreground=5;
  end
  xtitle("",_("Frequency (Hz)"),_("Phase (degree)"));
  // create legend
  if comments<>[] then
    c=captions(ephi.children,comments,"lower_caption")
    c.background=get(gcf(),"background")
  end
  fig.immediate_drawing=immediate_drawing;
  // return to the previous scale
  set( "current_axes", sciCurAxes ) ;

endfunction

function str=formatBodeMagTip(curve,pt,index)
//this function is called by the datatips mechanism to format the tip
//string for the magnitude bode curves
  str=msprintf("%.4g"+_("Hz")+"\n%.4g"+_("dB"), pt(1),pt(2))
endfunction

function str=formatBodePhaseTip(curve,pt,index)
//this function is called by the datatip mechanism to format the tip
//string for the bode phase curves
  str=msprintf("%.4g"+_("Hz")+"\n %.4g"+_("dB"), pt(1),pt(2))
endfunction
