function window_set_size(gh_window, viewport)
// Copyright INRIA
//** Set the size of the internal "virtual window workspace" of the physical graphic window


  // rhs = argn(2) ; //** get the number of right side arguments

  // if rhs==0 then //** without arguments (default) assume ...
     //** It is NOT possible to modify [gh_current_window] directly outside [scicos_new]
     // gh_curwin = gh_current_window ; //** get the handle of the current graphics window

  // else //** the arguments is explicit 
     //** It is NOT possible to modify [gh_current_window] directly outside [scicos_new]
     //gh_curwin = gh_window ; //** get the handle of the current graphics window

  // end

  if exists("gh_window","local")==0 then 
      gh_curwin = gh_current_window; //** using the semiglobal variable
  else
      gh_curwin = gh_window ; //** using the passing argument
  end 
  
  r = gh_curwin.figure_size ; //** acquire the current figure phisical size
                              //** figure_size: This property controls the size in pixel
                              //**  of the screen's graphics window.
                              //**             The size is the vector [width,height].m

  //** Scicos diagram size
  rect = dig_bound(scs_m) ;

//  rect(1),rect(4)            rect(3),rect(4)
//        +--------------------------+
//        |                          |
//        |                          |
//        +--------------------------+
//  rect(1),rect(2)            rect(3),rect(2)

  if rect==[] then //** if the schematics is not defined
      rect=[0,0,r(1),r(2)] ; //** take the figure physical size as reference
  end

  w = (rect(3)-rect(1));
  h = (rect(4)-rect(2));
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
  gh_axes = gh_curwin.children ; //** axes handle
  gh_axes.tight_limits = "on"  ; //** set the limit "gh_axes.data_bounds" in "hard mode"

  //** The default margin are [ 0.125 0.125 0.125 0.125 ]
  //** margins (default normalized values)
  //** Alan - No margins for axes please
  gh_axes.margins = [0,0,0,0] ;     //**

  //** Margins can be re-introduced here now
  margins = [0.02 0.02 0.02 0.02]
  wp=w*(ax+margins(1)+margins(2));
  hp=h*(ay+margins(3)+margins(4));
  xmin=rect(3)-wp*(bx+(1/ax))+margins(1)*wp;
  ymin=rect(4)-hp*(by+(1/ay))+margins(3)*hp;
  xmax=xmin+wp; ymax=ymin+hp;

  mrect = [xmin ymin ; xmax ymax] ; //** vector to matrix conversion
  gh_axes.data_bounds = mrect ; //** default : data_bounds = [xmin ymin ; xmax ymax ]

  wrect = [0 , 0, 1, 1] ;
  gh_axes.axes_bounds = wrect ; //** default : axes_bounds = [xleft yup width height] all include in [0,1]
                                //** xleft,yup : upper left corner
                                //** width height : ratio

  if exists("viewport", "local")==0 then 
    //** If no variable is passed 
    %XSHIFT = max( (width - r(1) ) / 2, 0) ;
    %YSHIFT = max( (height- r(2) ) / 2, 0) ;
    //** This correction is really needed BUT uses old graphics primitives :(
    if ~MSDOS then %YSHIFT = %YSHIFT+30 ; end //** correction for the UNIX system
    
    //** Beware ! : I'm forced to use old graphics instructions because there are not
    //**            direct equivalent inside the new graphics (24 may 2006)
    //**         
   
  else
    //** If the variable is passed  
    %XSHIFT = viewport(1) ;
    %YSHIFT = viewport(2) ;
    
    //** The function is NOT called beause the arguments are not modified ;)
  
  end 
  
   viewport = [%XSHIFT, %YSHIFT]; 
    set_viewport(viewport) ; //**

  xselect(); //** put the current window in foreground

endfunction

//**------------------------------------------------------------------------------------------------------
//**----------------------------- original code ------------------------------------
//function window_set_size()
//  [lhs,rhs]=argn(0)
//
//  r=xget('wpdim');
//  rect=dig_bound(scs_m);
//  if rect==[] then rect=[0,0,r(1),r(2)];end
//  w=rect(3)-rect(1);
//  h=rect(4)-rect(2);
//  j=min(1.5,max(1,1600/(%zoom*w)),max(1,1200/(%zoom*h)))  ;
//
//  ax=max(r(1)/(%zoom*w),j);
//  ay=max(r(2)/(%zoom*h),j);
//
//  xbasc()
//
//  xset("wresize",0);
//  width=%zoom*w*ax;height=%zoom*h*ay
//  xset('wdim',width,height);
//
//  bx=(1-1/ax)/2;
//  by=(1-1/ay)/2;
//  wrect=[bx,by,1/ax,1/ay]
//  arect=[0.02,0.02,0.02,0.02]
//
//  xsetech(wrect=wrect, frect=rect, arect=arect)
//

// wrect=wrect:
// The sub-window is specified with the parameter wrect=[x,y,w,h] (upper-left point, width, height).
// The values in wrect are specified using proportion of the width or height of the current graphic window.
// For instance wrect=[0,0,1,1] means that the whole graphics window will be used,
// and wrect=[0.5,0,0.5,1] means that the graphics region will be the right half of the graphics window.


// frect=rect
// frect=[xmin,ymin,xmax,ymax] is used to set the graphics scale and is just like the rect argument of plot2d.
// If frect is not given the current value of the graphic scale remains unchanged.
// The default value of rect is [0,0,1,1] (at window creation, when switching back to default value with
// xset('default') or when clearing graphic recorded events xbasc()).

// arect=arect
// arect=[x_left, x_right, y_up, y_down] is used to set the graphic frame inside the subwindow.
// The graphic frame is specified (like wrect) using proportion of the width or height of the current graphic
// subwindow. Default value is 1/8*[1,1,1,1]. If arect is not given, current value remains unchanged.

//  %XSHIFT=max((width-r(1))/2,0)
//  %YSHIFT=max((height-r(2))/2,0)
//  if ~MSDOS then %YSHIFT=%YSHIFT+30,end
//  xselect()
//  xset('viewport',%XSHIFT,%YSHIFT)
//endfunction



