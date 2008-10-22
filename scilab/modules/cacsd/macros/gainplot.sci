// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA Serge Steer
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
  fname="gainplot";//for error messages

  fmax=[];
  if or(typeof(varargin(1))==['state-space' 'rational']) then 
    //sys,fmin,fmax [,pas] or sys,frq
    refdim=1 //for error message
    if rhs==1 then
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
      [frq,d]=varargin(1:rhs-1)
      if size(frq,2)<>size(d,2) then
	error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same column dimensions expected.\n"),..
			 fname,1,2))
      end
    else 
      error(msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"),fname,2,4))
    end
  else
    error(msprintf(_("%s: Wrong type for input argument #%d: Linear dynamical system or row vector of floats expected.\n"),fname,1))
  end;

  frq=frq';d=d'
  [n,mn]=size(d)
  if and(size(comments,'*')<>[0 mn]) then
      error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same number of elements expected.\n"),...
		     fname,refdim,rhs+1))
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
  xtitle(' ',_("Frequency (Hz)"),_("Magnitude (Db)"));
  drawnow()
endfunction
