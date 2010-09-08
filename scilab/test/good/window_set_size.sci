//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

function window_set_size(gh_window, viewport)
//  Set the canvas  size according to a given diagram, also set the
//  viewport position (visible part of the canvas)

  if exists("gh_window","local")==0 then gh_window=gh_current_window,end
  gh_curwin = scf(gh_window) ; //** using the passing argument
  gh_axes = gca(); 
	
  r = gh_curwin.figure_size ; //** current figure physical size in pixels

  //** Scicos diagram size
  rect = dig_bound(scs_m) ;
  if rect==[] then //** if the diagram is empty
    rect=[0,0,r(1),r(2)] ; //** take the figure (viewport) size as reference
  end

//  (rect(1),rect(4))            (rect(3),rect(4))
//        +--------------------------+
//        |                          |
//        |                          |
//        +--------------------------+
//  (rect(1),rect(2))            (rect(3),rect(2))


  w = (rect(3)-rect(1));//width
  h = (rect(4)-rect(2));//height
  j = min(1.5,max(1,1600/(%zoom*w)),max(1,1200/(%zoom*h)))  ;

  ax = (max(r(1)/(%zoom*w),j)); //** amplitute correction if the user resize the window
  ay = (max(r(2)/(%zoom*h),j));
  bx = (1-1/ax)/2; //**
  by = (1-1/ay)/2; //**

  gh_curwin.auto_resize = "off" ; //**

  width  = %zoom * w * ax  ;   //** compute and set the axes dimensions
  height = %zoom * h * ay  ;

  gh_curwin.axes_size = [width height];

  //** axes settings
  gh_axes.tight_limits = "on"  ; //** set the limit "gh_axes.data_bounds" in "hard mode"

  //** ** TEXT use integer size / non aliased fonts ***
  gh_axes.fractional_font = "off";
  
  //** ** ARC/CIRCLE use line/segment ***
  gh_axes.arc_drawing_method = "lines"; 
  
  //** Alan - No margins for axes please
  gh_axes.margins = [0,0,0,0] ;     //**

  //** Margins can be re-introduced here now
  margins = [0.02 0.02 0.02 0.02]
  wp = w*(ax+margins(1)+margins(2));
  hp = h*(ay+margins(3)+margins(4));
  xmin = rect(3)-wp*(bx+(1/ax))+margins(1)*wp;
  ymin = rect(4)-hp*(by+(1/ay))+margins(3)*hp;
  xmax = xmin+wp;
  ymax = ymin+hp;

  mrect = [xmin ymin ; xmax ymax] ; //** vector to matrix conversion
  gh_axes.data_bounds = mrect ; //** default : data_bounds = [xmin ymin ; xmax ymax ]

  wrect = [0, 0, 1, 1] ; //use the entire axes rectangle 
  gh_axes.axes_bounds = wrect ; //** default : axes_bounds = [xleft yup width height] all include in [0,1]
                                //** xleft,yup : upper left corner
                                //** width height : ratio of the axes rectangle

  if exists("viewport", "local")==0 then 
    //** viewport variable not given
    %XSHIFT = max( (width - r(1) ) / 2, 0) ;
    %YSHIFT = max( (height- r(2) ) / 2, 0) ;
  else
    //** viewport variable given in the arg list
    %XSHIFT = viewport(1) ;
    %YSHIFT = viewport(2) ;
  end 
 
  gh_curwin.viewport = [%XSHIFT, %YSHIFT];
  show_window(); //** put the current window in foreground
endfunction

