function pwindow_set_size(gh_window)
// Copyright INRIA
//** Physical window set size 

  rhs = argn(2) ; //** get the number of right side arguments  
  
  if rhs==0 then //** without arguments (default) assume ... 
     //** It is NOT possible to modify [gh_current_window] directly outside [scicos_new]
     gh_curwin = gh_current_window ; //** get the handle of the current graphics window     
     
  else //** the arguments is explicit 
     //** It is NOT possible to modify [gh_current_window] directly outside [scicos_new]
     gh_curwin = gh_window ; //** get the handle of the current graphics window     
     
  end        
//

  rect = dig_bound(scs_m); //** acquire the coordinate of the rectangle that
                           //** include all the graphics object

//			   
//  rect(1),rect(4)            rect(3),rect(4)   
//	   +--------------------------+
//         |                          |	   
//	   |                          |	   
//         +--------------------------+
//  rect(1),rect(2)            rect(3),rect(2)
			     
  if rect<>[] then //** if defined  
    %diag_size = [rect(3)-rect(1), rect(4)-rect(2)]; //** real size of the real diagram
    %wsiz = [max(400,%diag_size(1)), max(300, %diag_size(2))]; //** minimum size for menus
  else
    %wsiz = [600/%zoom, 400/%zoom]; //** default values for empty diagram
  end
 
  //** Scale the window to the correct zoom factor and limit the maximum size 
  %wdd = min(933, %zoom*%wsiz(1)) + 30 ;
  %hdd = min(700, %zoom*%wsiz(2)) + 30 ;
  
  if ~MSDOS then
    %hdd=%hdd+50
  end
    
  //** set the the size of the phisical window on the screen (in pixel)
  //** with minimum w,h values  
  gh_curwin.figure_size = [ max(400,%wdd) max(300,%hdd) ] ; //** 

  //** N.B. : The "figure_size" controls ONLY the dimension of the phisical window:
  //**        the "real"dimension of the graphics window (relative to the zoom and factor is the)
  //**        "axes_size" parameters !
  
endfunction
