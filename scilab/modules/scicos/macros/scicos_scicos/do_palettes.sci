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

function [palettes,windows] = do_palettes(palettes, windows)


  global ClipboardPal
  
  if ClipboardPal==[] then
    //** Call a menu dialmog box in order to choose a palette 
    kpal = choose(scicos_pal(:,1),'Choose a Palette')
    
    //** if no Palette are choosen the exit 
    if kpal==0 then
        return ; //** EXIT point 
    end
  else
    kpal = ClipboardPal ;
    ClipboardPal = []   ; 
  end
  
  lastwin = curwin ; //** save the current graphic window_id 
  
  winpal = find(windows(:,1)==-kpal) ; //** look for the winid of the selected palette 
  
  //** First level protection 
  if winpal<>[] then
    
    if ~or(windows(winpal,2)==winsid()) then
      windows(winpal,:)=[]
      winpal=[]
    end
  
  end

  //** Second level protection
  if winpal==[] then  // selected palettes isn't loaded yet
    
    curwin = get_new_window(windows)
    
    windows = [windows; [-kpal curwin] ];
    palettes = add_palette(palettes, scicos_pal(kpal,2),kpal); 
    
    if palettes(kpal)==list() then
      return; //** EXIT point 
    end
  
  else //** selected palettes is already loaded BUT the user could
       //** have manually closed the window  
  
    curwin = windows(winpal,2) ;
  
  end

  //** Alan : no grid for palette
  %scicos_with_grid = %f ;
  
  gh_current_window = scf(curwin) ; //** open a graphic window and get the handler
  gh_curwin = gh_current_window   ;
  gh_axes   = gca()               ;

  if gh_axes.children<>[] then
    //** the palette is already present on the screen  
    return;
  end 

  gh_palette  = gh_curwin          ;
  gh_pal_axes = gh_axes            ;
  
  //** Remove the default Scilab graphics window menus 
  //** but leave the "File" menu active for export 
  delmenu(curwin,_("&Edit"))
  delmenu(curwin,_("&Tools"))
  delmenu(curwin,_("&?"))
  
  toolbar(curwin, "off"); //** by Vincent C.
  xselect(); //** rise the current graphics window 
 
  //**-------------------------------------------------------

  rect = dig_bound(palettes(kpal));
  
  if rect==[] then 
    rect = [0 0 400,600]; //** default palette size
  end
  
  %wsiz = [rect(3)-rect(1),rect(4)-rect(2)];
  // window size is limited to 400 x 300 : ajust dimensions
  // to remain isometric.
  
  if %wsiz(1)<400 then 
    rect(1) = rect(1)-(400-%wsiz(1))/2 ;
    rect(3) = rect(3)+(400-%wsiz(1))/2 ;
    %wsiz(1)= 400; 
  end

  if %wsiz(2)<300 then 
    rect(2) = rect(2)-(300-%wsiz(2))/2 ;
    rect(4) = rect(4)+(300-%wsiz(2))/2 ;
    %wsiz(2)= 300; 
  end

  %zoom = 1.2
  h = %zoom*%wsiz(2)
  w = %zoom*%wsiz(1)

  if ~MSDOS then
    h1 = h + 50 ; //** Linux 
  else
    h1 = h ;  //** Windows 
  end
  
//  xset('wresize',1); //** xset("wresize",flag). If flag=1 then the graphic is automatically resized
//                     //** to fill the graphics window. 
  
//  xset('wpdim',w,h1);//** xset("wpdim",width,height): Sets the width and the height of the current
//                     //** physical graphic window (which can be different from the actual size in 
//                     //** mode wresize 1). This option is not used by the postscript driver.  


  gh_palette.figure_size = [w h1] ; 

  //** sleep(50); //** workaround fro JB bug on graphics proprieties 
    
//  xset('wdim',w,h)   //** xset("wdim", width, height): Set the width and the height of the current
//                     //** graphics window. This option is not used by the postscript driver. 

  gh_palette.axes_size = [w h];


//  xsetech(wrect=[0 0 1 1] ; frect=rect, arect=[1 1 1 1]/32 ) ; 

  wrect = [0 0 1 1] ; //** default value 
  frect = [rect(1) rect(2) ; rect(3) rect(4)] ; //** vector to matrix conversion       ; 
  arect = [1 1 1 1] / 32 ; //** 1/32 of margin  
 
  gh_pal_axes.axes_bounds = wrect ; //** default : axes_bounds = [0,0,1,1] = [xmin ymin xmax ymax] 
 
  gh_pal_axes.data_bounds = frect ; //** default : data_bounds = [0,0 ; 1,1] = [xmin ymin ; xmax ymax ]
  
  gh_pal_axes.margins     = arect ; //** arect
 
  gh_pal_axes.mark_mode = "off"   ; //** 
  gh_pal_axes.box = "off"         ; //** 
  gh_pal_axes.tight_limits = "on" ; //** 

  options = palettes(kpal).props.options ;
    
  //** I switch to the direct access 
  gh_palette.background = options.Background(1) ; //** "options" is sub structure of scs_m  
     
  if ~set_cmap(palettes(kpal).props.options('Cmap')) then 
       palettes(kpal).props.options('3D')(1)=%f //disable 3D block shape 
  end
      
  drawobjs(palettes(kpal), gh_palette); //** draw all the object of the palettes 
  
  xinfo("The Palette can be used to copy blocks or regions");   
  
  //** force the proprieties of the palette windows:
  //** the user should be capable to change the size of the window   
  gh_palette.auto_resize = "on"       ; //** 
  gh_palette.pixmap      = "off"      ;
  gh_palette.immediate_drawing = "on" ;

  //** put the focus in the previous window. 
  set ("current_figure" , lastwin ); //** new graphic 

endfunction
