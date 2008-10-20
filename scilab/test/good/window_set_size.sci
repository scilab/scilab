//** 24 May 2006 
//** Modified by Simone Mannori 09 Jan 2006 

//** Set the size of the internal "virtual window workspace" of the phisical graphic window

function window_set_size()

  //** [lhs,rhs] = argn(0) //** removed : NEVER USED
  
  
  //** NEW graphics
  //** It is NOT possible to modify [gh_current_window] directly outside [scicos_new]
  gh_curwin = gh_current_window ; //** get the handle of the current graphics window
  
  //** r = xget('wpdim'); 
  //** xset("wpdim",width,height): Sets the width and the height of the current physical graphic window
  //**                             (which can be different from the actual size in mode wresize 1). 
  
  r = gh_curwin.figure_size ; //** acquire the current figure phisical size 
  //**             figure_size: This property controls the size in pixel of the screen's graphics window.
  //**             The size is the vector [width,height]. 
 
  rect = dig_bound(scs_m) ;

  //** disp ("Window parameters"); disp (rect) ; //** debug only 
  
  if rect==[] then rect=[0,0,r(1),r(2)] ; end //** 
  
  w = rect(3)-rect(1);
  h = rect(4)-rect(2);
  j = min(1.5,max(1,1600/(%zoom*w)),max(1,1200/(%zoom*h)))  ;
  ax= max(r(1)/(%zoom*w),j);
  ay= max(r(2)/(%zoom*h),j);

//** xbasc(); //** obsolete, removed 

//** clf(); //** 

//**   xset("wresize",0);
//**   xset("wresize",flag) If flag=1 then the graphic is automatically resized to fill the graphics
//**                        if flag=0 the scale of the graphic is left unchanged when the graphics window is resized.
//**                        Top left panner or keyboard arrows may be used to scroll over the graphic.  

  gh_curwin.auto_resize = "off" ; //** I'm not 100% sure about that 
  
  width = %zoom * w * ax; height = %zoom * h * ay
  
  //** xset('wdim',width,height);
  
  gh_curwin.axes_size = [width height];
  
  //** The default margin are [ 0.125 0.125 0.125 0.125 ]
  arect = [0.02,0.02,0.02,0.02] //** margins (default normalized values)
  
  //** 
  mrect = [rect(1) rect(2) ; rect(3) rect(4)] ; //** vector to matrix conversion   
  
  bx = (1 - 1/ax)/2; by=(1 - 1/ay)/2;
  
  wrect = [bx , by, 1/ax, 1/ay]
  
  //** xsetech(wrect=wrect,frect=rect,arect=arect) //** bastard operation //** obsolete, removed 
 
  gh_axes = gh_curwin.children ;
  
  //** some problems around here <----------------------
  
  gh_axes.margins     = arect ; //** arect=[0.02,0.02,0.02,0.02]
  
  gh_axes.data_bounds = mrect ; //** default : data_bounds = [0,0 ; 1,1] = [xmin ymin ; xmax ymax ]
  
  gh_axes.axes_bounds = wrect ; //** default : axes_bounds = [0,0 , 1,1] = [xmin ymin xmax ymax] 
    
  %XSHIFT = max( (width - r(1) ) / 2, 0) ;
  %YSHIFT = max( (height- r(2) ) / 2, 0) ;
  
  //** This correction is really needed BUT uses old graphics primitives :(
  //if ~MSDOS then %YSHIFT = %YSHIFT+30 ; end //** correction for the UNIX system 
  
  //** Beware ! : I'm forced to use a couple of old graphics instructions because there are not 
  //**            direct equivalent inside the new graphics (24 may 2006) 
  
  xset('viewport', %XSHIFT, %YSHIFT) ; //** xset("viewport",x,y): Set the position of the panner. 

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
//  ax=max(r(1)/(%zoom*w),j);
//  ay=max(r(2)/(%zoom*h),j);
//
//  xbasc()
//
//  xset("wresize",0);
//  width=%zoom*w*ax;height=%zoom*h*ay
//  xset('wdim',width,height);
//  bx=(1-1/ax)/2;
//  by=(1-1/ay)/2;
//  wrect=[bx,by,1/ax,1/ay]
//  arect=[0.02,0.02,0.02,0.02]
//
//  xsetech(wrect=wrect,frect=rect,arect=arect)
//
//  %XSHIFT=max((width-r(1))/2,0)
//  %YSHIFT=max((height-r(2))/2,0)
//  if ~MSDOS then %YSHIFT=%YSHIFT+30,end
//  xselect()
//  xset('viewport',%XSHIFT,%YSHIFT)
//endfunction



