//** Modified by Simone Mannori

//** Phisical window set size 

function pwindow_set_size()
  
  rect = dig_bound(scs_m); //** acquire the coordinate of the rectangle that
                           //** include all the graphics object 
  
  if rect<>[] then 
    %wsiz=[rect(3)-rect(1),rect(4)-rect(2)];
  else
    %wsiz=[600/%zoom, 400/%zoom]
  end

  %wdd = min(1200,%zoom*%wsiz(1)) + 30 ;
  %hdd = min(1000,%zoom*%wsiz(2)) + 30 ;
  
  if ~MSDOS then
    %hdd=%hdd+50
  end
  
  //** xset('wpdim',min(1200,%wdd),min(1000,%hdd))
  //** xset("wpdim",width,height): Sets the width and the height of the current physical graphic window
  //**                              (which can be different from the actual size in mode wresize 1).
  
  
  
  //** NEW graphics
  //** It is NOT possible to modify [gh_current_window] directly outside [scicos]
  gh_curwin = gh_current_window ; //** get the handle of the current graphics window
  // gh_curwin.figure_size = [ min(1200,%wdd) min(1000,%hdd) ] ; //** 
  gh_curwin.figure_size = [ max(400,%wdd) max(300,%hdd) ] ; //** 
endfunction
