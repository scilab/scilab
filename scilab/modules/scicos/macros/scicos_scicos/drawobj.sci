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

function gh_blk = drawobj(o, gh_window)
//
//!
//**    
//
//** 22 May 2006 : this function IS substantially different from the standard
//**               SCICOS because came back with the graphic handler of 
//**               the compount that contains all the object(s) drawn. 
//**
//** 07 June 2006: adding of an empty box for the Select / HighLith operations 
//**
//** 22 Aug 2006: adding text support 
//**
//** 21 Nov 2006: Save / Restore graphics attribute mechanism
//**
//** 23 Nov 2006: Faster Save/Restore, new "flat" object selection mechanism
//**
//** 21/07/07: Al@n's patch for rotation of text
//** 24/07/07: Al@n's patch for rotation of blocks
//**
  rhs = argn(2) ; //** get the number of right side arguments

  if rhs==1 then //** without arguments (default) assume ...
     //** It is NOT possible to modify [gh_current_window] directly outside [scicos_new]
     gh_curwin = scf(curwin);
     gh_axes = gca(); 

  else //** the arguments is explicit
     //** It is NOT possible to modify [gh_current_window] directly outside [scicos_new]
     gh_curwin = scf(gh_window) ; //** get the handle of the current graphics window
     gh_axes = gca(); 
  end

  gh_blk = [] ; //** create the empty object value

  //** moved to "drawobjs()"
  mark_size = int(%zoom * 3.0); //** in pixel : size of the selection square markers

  if typeof(o)=="Block" then //** Block draw 
    //** ---------------------- Block -----------------------------

    o_size = size ( gh_axes.children ) ; //** initial size (number of graphic object


   //** Save graphics axes state
      //** Save the state of the graphics window to avoid problems in case of not "well behaved"
      //** incorrect "gr_i" old graphics instructions

      //** figure_background = gh_curwin.background  ;

      axes_font_style = gh_axes.font_style ;
      axes_font_size  = gh_axes.font_size  ;
      axes_font_color = gh_axes.font_color ; //** optional

      //** axes_line_mode   = gh_axes.line_mode ; //** optional
      axes_line_style  = gh_axes.line_style  ;
      axes_thickness   = gh_axes.thickness   ;

      //** axes_mark_mode       = gh_axes.mark_mode       ;
      //** axes_mark_style      = gh_axes.mark_style      ;
      //** axes_mark_size       = gh_axes.mark_size       ;
      //** axes_mark_foreground = gh_axes.mark_foreground ; 
      //** axes_mark_background = gh_axes.mark_background ; 

      axes_foreground = gh_axes.foreground ;
      axes_background = gh_axes.background ; //** if not used cause some problem

   //**... end of figure and axes saving

        //** Block drawing works throught call (execstr) the block function
        //** ... see "standard_draw" function
        //** WARINING: this indirect "gr_i" execution can ruin the axis graphics proprieties because a not
        //**           formaly correct use of OLD graphics global primitives with "xset(..,..)"
        ierr = execstr(o.gui+'(''plot'',o);','errcatch')

        [orig, sz, orient] = (o.graphics.orig, o.graphics.sz, o.graphics.flip) ;

        //** Add the 'select' box and put "mark_mode" off, ready for 'Select' operation
        sel_x = orig(1) ; sel_y = orig(2)+sz(2) ;
        sel_w = sz(1)   ; sel_h = sz(2)   ;

        xrect(sel_x, sel_y, sel_w, sel_h); //** draw the selection box 

        gh_e = gce()                  ; //** get the "select box" handle
         gh_e.mark_size_unit = "point" ;
         gh_e.mark_size = mark_size    ; // size of the square selection boxes
         gh_e.mark_style = 11          ; //** boxes
         gh_e.mark_background = -1     ; //** filled boxes 
         gh_e.mark_mode = "off"        ; //** put it off 
         gh_e.line_mode = "off"        ; //** idem 

   //** Restore graphics axes state 
      //** Restore the state of the graphics window 

      gh_axes.font_style = axes_font_style ;
      gh_axes.font_size  = axes_font_size  ;
      gh_axes.font_color = axes_font_color ; //** optional

      //** gh_axes.line_mode   = axes_line_mode   ; //** optional
      gh_axes.line_style  = axes_line_style  ;
      gh_axes.thickness   = axes_thickness   ;

      //** gh_axes.mark_mode       = axes_mark_mode       ; //** optional
      //** gh_axes.mark_style      = axes_mark_style      ;
      //** mark_size_unit = "tabulated"                   ; 
      //** gh_axes.mark_size       = axes_mark_size       ;
      //** gh_axes.mark_foreground = axes_mark_foreground ; //** optional
      //** gh_axes.mark_background = axes_mark_background ; //** optional

      gh_axes.foreground = axes_foreground ;
      gh_axes.background = axes_background ;

      //** gh_curwin.background = figure_background ;

   //**... end of figure and axes state restoring

    p_size = size(gh_axes.children); //** size after the draw
    //** aggregate the graphics entities
    d_size =  p_size(1) - o_size(1);

    gh_blk = glue(gh_axes.children(d_size:-1:1));

    //** 24/07/07: Al@n's patch for rotation of blocks
    if o.graphics.theta<>0 then
      rotate_compound(sel_x, sel_y, sel_w, sel_h,1, o.graphics.theta);
      gh_blk = gh_axes.children(1);
    end
    //** Block rotation end 

    if ierr<>0 then 
      message(['Problem loading block '+o.gui'; lasterror();
	       'Use Activate_Scilab_Window and redefine it in Scilab.'] ) ;
      gh_blk = [];
    end
    



  //** ---------- Link -------------------------------
  elseif typeof(o)=="Link" then //** Link draw 

    o_size = size ( gh_axes.children ) ; //** initial size

        xpoly(o.xx, o.yy,'lines')  ; //** draw the polyline "Link"
        gh_e = gce()               ;
         gh_e.thickness = maxi( o.thick(1) , 1) * maxi(o.thick(2), 1) ; //** thickness
         gh_e.foreground = o.ct(1)  ; //** link color
         gh_e.mark_size_unit = "point" ;
         gh_e.mark_size = mark_size    ; // size of the square selection boxes
         gh_e.mark_style = 11          ; //** boxes
         gh_e.mark_mode = "off"        ; //** put "mark_mode" off, ready for 'Select' operation

    p_size = size ( gh_axes.children ) ; //** size after the draw
    //** aggregate the graphics entities
    d_size = p_size(1) - o_size(1) ;
    gh_blk = glue( gh_axes.children(d_size:-1:1) ) ;

  //** ---------- Deleted ----- CAUTION: also "Deleted" object MUST be draw ! ----
  elseif typeof(o)=="Deleted" then //** Dummy "deleted" draw

    //** draw a dummy object

    xpoly ([0;1],[0;1]) ; //** just a dummy object
    gh_blk = glue( gh_axes.children(1) ) ; //** create the Compound macro object

    //** gh_blk.visible = "off"  ; //** set to invisible :)
    set (gh_blk,"visible","off");  //** set to invisible -> faster version


  //** ---------- Text -------------------------------
  elseif typeof(o)=="Text" then //** Text draw
    //**  ------ Put the new graphics here -----------
    //**
    o_size = size ( gh_axes.children ) ; //** initial size
    exe_string = o.gui+'(''plot'',o)' ;
    execstr(o.gui+'(''plot'',o)') ;
   
    //** "stringbox" in ver 4.1 it is not compatible with multiple string
    //** plot because is not able to handle "Compound" graphics object
    //** This code patch the limited functionality of stringbox() with 
    //** a double loop that scan all the elements of the Compound in order to
    //** compute the rectangle that include all the strings  
    //** rect = stringbox(gh_curwin.children.children(1))                     ;
    //** xrect(rect(1,2),rect(2,2),rect(1,3)-rect(1,2),rect(2,2)-rect(2,4)) ;

    if gh_axes.children(1).type == "Compound" then

       matrix_dim = size(gh_axes.children(1).children)
       imax = matrix_dim(1); jmax = matrix_dim(2);
       xvec = [] ; yvec = [] ; 
       for i=1:imax
          for j=1:jmax
	    rect = stringbox( gh_axes.children(1).children(i,j) ) 
            xvec = [xvec rect(1,2) rect(1,3)]
	    yvec = [yvec rect(2,2) rect(2,4)]
	  end
       end
       xmin = min(xvec) ; xmax = max(xvec) ;
       ymin = min(yvec) ; ymax = max(yvec) ;
       xrect( xmin, ymax, xmax-xmin, ymax-ymin ) ;

    else //** single string
       //** 21/07/07: Al@n's patch for rotation of text
       //** get the handle of txt
       gh_txt = gh_axes.children(1)
       //** get bounding box of text with no rotation
       rect = stringbox(gh_txt)
       x1=rect(1,2);y1=rect(2,2);w=rect(1,3)-rect(1,2);h=rect(2,2)-rect(2,4);
       //** compute new bounding with rotate
       //** (rotation is done with the center of the bounding box)
       xxx=rotate([x1,x1,x1+w,x1+w;y1,y1-h,y1-h,y1],o.graphics.theta*%pi/180,[x1+w/2;y1-h/2])
       //** update lower left point of text
       gh_txt.data = [xxx(1,2) xxx(2,2)]
       //**adjust theta according to gh_txt.font_angle
       if o.graphics.theta > 0 then
         gh_txt.font_angle = 360 - o.graphics.theta
       else
         gh_txt.font_angle = -o.graphics.theta
       end
       //** compute new bounding box with rotation
       rect = stringbox(gh_txt)
       //** draw a polyline for the "select box"
       xpoly(rect(1,:),rect(2,:))

    end

    gh_e = gce()                  ; //** get the "select box" handle
     gh_e.mark_size_unit = "point" ;
     gh_e.mark_size = mark_size    ; // size of the square selection boxes
     gh_e.mark_style = 11          ; //** boxes
     gh_e.mark_background = -1     ; //** filled boxes 

    gh_e.mark_mode = "off"        ; //** put it off 
    gh_e.line_mode = "off"        ; //** idem 

    p_size = size ( gh_axes.children ) ; //** size after the draw
    //** aggregate the graphics entities
    d_size =  p_size(1) - o_size(1) ;

    gh_blk = glue( gh_axes.children(d_size:-1:1) ) ;
  
 end //** of the main if

endfunction
