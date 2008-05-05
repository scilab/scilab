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

function [palettes,windows] = do_load_as_palette(palettes, windows)
//** 
//**    
//**   
  [ok,scs_m,cpr,edited] = do_load() ;
  
  if ~ok then return, end //** if fail --> Exit 

  maxpal = -mini([-200;windows(:,1)]) ; //** look for the last valid palette 
   
  kpal = maxpal+1  ; //** add one 

  lastwin = curwin ;  //** save the current active win_id 

  curwin = get_new_window(windows) //** get a new win_id 
  
  if or(curwin==winsid()) then //** protection: if the new win_id is already in the list              
    clf(curwin)  ;  //** delete it !
  end
  
  windows = [windows;[-kpal curwin]] ; //** add the new window ad the "windows" list as palette
                                       //** with (NEGATIVE id) "-kpal"
  
  palettes(kpal) = scs_m ; //** save the diagram in the datastructure 
  
  gh_curwin = scf(curwin) ; //** open the new palette windows with proper id 
  gh_axes = gca(); 

  //** Remove the default Scilab graphics window menus 
  //** but leave the "File" menu active for export 
  delmenu(curwin,_("&Edit"))
  delmenu(curwin,_("&Tools"))
  delmenu(curwin,_("&?"))

  toolbar(curwin, "off"); //** by Vincent C.
  
  rect = dig_bound(scs_m);

  if rect==[] then rect=[0 0 400,600],end
  %wsiz=[rect(3)-rect(1),rect(4)-rect(2)];
  // window size is limited to 400 x 300 ajust dimensions
  // to remain isometric.
  
  if %wsiz(1)<400 then //** if the "X" is smaller than ... then 
    rect(1)=rect(1)-(400-%wsiz(1))/2
    rect(3)=rect(3)+(400-%wsiz(1))/2
    %wsiz(1)=400 
  end
  
  if %wsiz(2)<300 then //** if the "Y" is smaller than ... then 
    rect(2)=rect(2)-(300-%wsiz(2))/2
    rect(4)=rect(4)+(300-%wsiz(2))/2
    %wsiz(2)=300 
  end

  %zoom = 1.2 ; //** default zoom value 
  h = %zoom * %wsiz(2) ;
  w = %zoom * %wsiz(1) ;

  if ~MSDOS then 
    h1 = h + 50 ;
  else
    h1 = h
  end //** correction for Unix machines 
  
  //** xset('wresize',1); //** If flag=1 then the graphic is automatically resized to fill the graphics
  
  gh_curwin.auto_resize = "on" ; //** for the palette window(s) the auto resize is active 
  
  //** xset('wpdim',w,h1) ; //** Sets the width and the height of the current physical graphic window
                            //** (which can be different from the actual size in mode wresize 1). 
  gh_curwin.axes_size = [w h1] ; 
 
  //** xset('wdim',w,h)           //** Set the width and the height of the current graphics window.
  gh_curwin.figure_size = [w h] ; 
  
  //** axes settings 
  //** gh_axes = gh_curwin.children ; //** axes handle
  gh_axes.tight_limits = "on"  ; //** set the limit "gh_axes.data_bounds" in "hard mode"
  

  //** xsetech(wrect=[0 0 1 1],frect=rect,arect=[1 1 1 1]/32)
  
  //** The default margin are [ 0.125 0.125 0.125 0.125 ]
  arect = [1.0 1.0 1.0 1.0 ] / 32.0  //** margins (default normalized values)
  gh_axes.margins = arect ;          //**
  
  wrect = [0.0 0.0 1.0 1.0]   ; //** uses the full window space 
  gh_axes.axes_bounds = wrect ; //** default : axes_bounds = [0,0 , 1,1] = [xmin ymin xmax ymax] 
  
  //** map the diagram size on the window size
  mrect = [rect(1) rect(2) ; rect(3) rect(4)] ; //** vector to matrix conversion   
  gh_axes.data_bounds = mrect ; //** default : data_bounds = [0,0 ; 1,1] = [xmin ymin ; xmax ymax ]
  
  if ~set_cmap(palettes(kpal).props.options('Cmap')) then 
    palettes(kpal).props.options('3D')(1)=%f //disable 3D block shape 
  end
  
  options = palettes(kpal).props.options
  
  set_background(gh_curwin); 
  
  drawobjs(palettes(kpal),gh_curwin)
  
  xinfo("The Palette can be used to copy blocks or regions"); 
  
  drawnow();

  scf(lastwin) ;
  
endfunction
