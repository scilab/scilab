// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function plzr(a,b,c,d)
//
// Copyright INRIA
  [lhs,rhs]=argn(0)
  select typeof(a)
  case 'rational' then
    if rhs<>1 then 
      error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"plzr",1)),
    end
    a=tf2ss(a),
    [a,b,c,d]=a(2:5)
    if type(d)<>1 then 
      error(msprintf(gettext("%s: Wrong value for of input argument %d: Proper system expected.\n"),"plzr",1));
    end
  case 'state-space' then
    if rhs<>1 then 
      error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"plzr",1)),
    end
    [a,b,c,d]=a(2:5)
    if type(d)<>1 then 
      error(msprintf(gettext("%s: Wrong value for of input argument %d: Proper system expected.\n"),"plzr",1));
    end
  case 'constant' then
    if rhs<>4 then 
      error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"plzr",4)),
    end
    if type(d)<>1 then 
      error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of floating point numbers expected.\n"),..
		     "plzr",4));
    end

  else
    if rhs==1 then
      error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear dynamical system expected.\n"),"plzr",1))
    else
      error(msprintf(gettext("%s: Wrong type of input argument #%d: Array of floating point numbers expected.\n"),"plzr",1))
    end
  end
  if type(d)<>1 then 
    error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of floating point numbers expected.\n"),..
		   "plzr",4));
  end


  dr=spec(a)
  [al,be]=tr_zer(a,b,c,d)
  nr=al./be
  ni=imag(nr);nr=real(nr)
  di=imag(dr);dr=real(dr)
  //
  mxx=maxi([nr;dr;1]*1.1)
  mnx=mini([nr;dr;-1]*1.1)
  my=maxi(abs([ni;di;1])*1.1)
 
  rect=[mnx, -my, mxx, my];

  wdim=get(gcf(),'axes_size')
  dx=(mxx-mnx)/wdim(1);dy=2*my/wdim(2)
  if dy>dx then 
    ax=(dy*wdim(1)-mxx+mnx)/2
    mxx=mxx+ax;mnx=mnx-ax
  elseif dy<dx then
    ay=(dx*wdim(2)-2*my)/2
    my=my+ay
  end
  rect=[mnx, -my, mxx, my];
  drawlater()
  ax=gca();
  ax.data_bounds=[mnx, -my; mxx, my];
  ax.axes_visible='on';
 
  legs=[],lhandle=[]
  if size(nr,'*')<>0 then
    xpoly(nr,ni)
    e=gce();e.line_mode='off';e.mark_mode='on';
    e.mark_size_unit="point";e.mark_size=7;e.mark_style=5;
    legs=[legs;gettext("Zeros")]
    lhandle=[lhandle;e]
  end;
  if size(dr,'*')<>0 then
    xpoly(dr,di)
    e=gce();e.line_mode='off';e.mark_mode='on';
    e.mark_size_unit="point";e.mark_size=7;e.mark_style=2;
    legs=[legs;gettext("Poles")]
    lhandle=[lhandle;e]
  end
  ax.grid=ones(1,3)*color('gray')
  ax.box = "on";
  xarc(-1,1,2,2,0,360*64)
  xtitle(gettext("Transmission zeros and poles"),gettext("Real axis"), ...
	 gettext("Imaginary axis"));
  if legs<>[] then legend(lhandle,legs,1),end
  drawnow()
  xselect();
endfunction
