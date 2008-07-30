
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function ge_set_winsize()
  %zoom=EGdata.Zoom
  gh_curwin = gcf()
  if exists('%wsiz')==1 &%wsiz<>[] then gh_curwin.figure_size=%wsiz,end
  r = gh_curwin.figure_size ; 


  //** diagram size
  rect = ge_dig_bound(GraphList);

  w = (rect(3)-rect(1));
  h = (rect(4)-rect(2));

  j = min(1.5,max(1,1600/(%zoom*w)),max(1,1200/(%zoom*h)))  ;
  ax = (max(r(1)/(%zoom*w),j)); //** amplitute correction if the user  resize the window
  ay = (max(r(2)/(%zoom*h),j));
  width  = %zoom * w * ax  ;   //** compute and set the axes dimensions
  height = %zoom * h * ay  ;
  bx = (1-1/ax)/2; 
  by = (1-1/ay)/2; 

  gh_curwin.auto_resize = "off" ; 
  gh_curwin.axes_size = [width height];

  //** axes settings
  gh_axes = gca(); 
  gh_axes.tight_limits = "on"  ; 
  gh_axes.margins = [0,0,0,0] ;   

  //** Margins can be re-introduced here now
  margins = [0.02 0.02 0.02 0.02]
  wp=w*(ax+margins(1)+margins(2));
  hp=h*(ay+margins(3)+margins(4));
  xmin=rect(3)-wp*(bx+(1/ax))+margins(1)*wp;
  ymin=rect(4)-hp*(by+(1/ay))+margins(3)*hp;
  xmax=xmin+wp; ymax=ymin+hp;

  mrect = [xmin ymin ; xmax ymax] ; //** vector to matrix conversion
  gh_axes.data_bounds = mrect ; 

  wrect = [0 , 0, 1, 1] ;
  gh_axes.axes_bounds = wrect ;
  //** xleft,yup : upper left corner
  //** width height : ratio

  %XSHIFT = max( (width - r(1) ) / 2, 0) ;
  %YSHIFT = max( (height- r(2) ) / 2, 0) ;

  if ~MSDOS then %YSHIFT = %YSHIFT+30 ; end //** correction for the UNIX system
  ge_set_viewport(%XSHIFT, %YSHIFT) ; 

  xselect(); 

endfunction

