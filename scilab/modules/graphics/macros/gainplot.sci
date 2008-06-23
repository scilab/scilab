// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function []=gainplot(varargin)
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
		     "gainplot",1,1))
      end
      [frq,repf]=repfreq(varargin(1:rhs))
    elseif or(rhs==(3:4)) then //sys,fmin,fmax [,pas]
      [frq,repf]=repfreq(varargin(1:rhs))
    else
      error(msprintf(_("%s : Invalid call: sys,fmin,fmax [,pas] [,com]'),"gainplot"))
    end
    [phi,d]=phasemag(repf)
  elseif  type(varargin(1))==1 then 
    //frq,db,phi [,comments] or frq, repf [,comments]
    select rhs
    case 2 then //frq,repf
      frq=varargin(1);
      if size(frq,2)<2 then
	error(msprintf(_("%s : Invalid argument #%d. It must be a row vector with length > %d"),..
		     "gainplot",1,1))
      end
      if size(frq,2)<>size(varargin(2),2) then
	error(msprintf(_("%s : Incompatible dimensions of arguments #%d and #%d."),..
			 "gainplot",1,2))
      end
 
      [phi,d]=phasemag(varargin(2))
    case 3 then  //frq,db,phi
      [frq,d]=varargin(1:rhs-1)
      if size(frq,2)<>size(d,2) then
	error(msprintf(_("%s : Incompatible dimensions of arguments #%d and #%d."),..
			 "gainplot",1,2))
      end
    else 
      error(msprintf(_("%s : Invalid call: frq, db,phi [,com] or frq,repf [,com]'),"gainplot"))
    end
  else
    error(msprintf(_("%s : Invalid argument #%d. It must be a linear"+..
		     " dynamical system or a real array"),"gainplot",1))
  end;

  frq=frq';d=d'
  [n,mn]=size(d)
  if and(size(comments,'*')<>[0 mn]) then
    error(msprintf(_("%s : Invalid dimension for argument #%d"),"gainplot",rhs+1))
  end

  //
  drawlater()
  axes = gca() ;axes.axes_visible='on';
  axes.data_bounds = [mini(frq),mini(d);maxi(frq),maxi(d)];
  axes.log_flags = "lnn" ;
  axes.grid=color("lightgrey")*ones(1,3);
  
  if size(d,2)>1&size(frq,2)==1 then
    xpolys(frq(:,ones(1,mn)),d,1:mn)
  else
    xpolys(frq,d,1:mn)
  end
  if comments<>[] then 
    legend(comments)
  end
  xtitle(' ','Hz','Db');
  drawnow()
endfunction
